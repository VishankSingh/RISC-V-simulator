/**
 * @file vm_base.cpp
 * @brief File containing the base class for the virtual machine
 * @author Vishank Singh, https://VishankSingh
 */

#include "vm_base.h"

#include "../globals.h"



void VmBase::LoadProgram(const AssembledProgram &program) {
    unsigned int counter = 0;
    for (const auto &instruction: program.text_buffer) {
      memory_controller_.WriteWord(counter, static_cast<uint32_t>(instruction.to_ulong()));
        counter += 4;
    }

    program_size_ = counter;
  AddBreakpoint(program_size_);

    unsigned int data_counter = 0;
    uint64_t base_data_address = globals::data_section_start;
    for (const auto& data : program.data_buffer) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;  // Deduce the type of the value
            if constexpr (std::is_same_v<T, uint8_t>) {
              memory_controller_.WriteByte(base_data_address + data_counter, value);  // Write a byte
                data_counter += 1;
            } else if constexpr (std::is_same_v<T, uint16_t>) {
              memory_controller_.WriteHalfWord(base_data_address + data_counter, value);  // Write a halfword (16 bits)
                data_counter += 2;
            } else if constexpr (std::is_same_v<T, uint32_t>) {
              memory_controller_.WriteWord(base_data_address + data_counter, value);  // Write a word (32 bits)
                data_counter += 4;
            } else if constexpr (std::is_same_v<T, uint64_t>) {
              memory_controller_.WriteDoubleWord(base_data_address + data_counter, value);  // Write a double word (64 bits)
                data_counter += 8;
            } else if constexpr (std::is_same_v<T, float>) {
                uint32_t float_as_int;
                std::memcpy(&float_as_int, &value, sizeof(float));
              memory_controller_.WriteWord(base_data_address + data_counter, float_as_int);  // Write the float as a word
                data_counter += 4;
            } else if constexpr (std::is_same_v<T, double>) {
                uint64_t double_as_int;
                std::memcpy(&double_as_int, &value, sizeof(double));
              memory_controller_.WriteDoubleWord(base_data_address + data_counter, double_as_int);  // Write the double as a double word
                data_counter += 8;
            } else if constexpr (std::is_same_v<T, std::string>) {
                for (size_t i = 0; i < value.size(); i++) {
                  memory_controller_.WriteByte(base_data_address + data_counter, static_cast<uint8_t>(value[i]));  // Write each byte of the string
                    data_counter += 1;
                }
            }
        }, data);
    }


}

uint64_t VmBase::GetProgramCounter() const {
    return program_counter_;
}

void VmBase::UpdateProgramCounter(int64_t value) {
    program_counter_ = static_cast<uint64_t>(static_cast<int64_t>(program_counter_) + value);
}

auto sign_extend = [](uint32_t value, unsigned int bits) -> int32_t {
    int32_t mask = 1 << (bits - 1);
    return (value ^ mask) - mask;
};


int32_t VmBase::ImmGenerator(uint32_t instruction) {
    int32_t imm = 0;
    uint8_t opcode = instruction & 0b1111111;

    switch (opcode) {
        /*** I-TYPE (Load, alu Immediate, JALR, FPU Loads) ***/
        case 0b0010011: // alu Immediate (ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI)
        case 0b0000011: // Load (LB, LH, LW, LD, LBU, LHU, LWU)
        case 0b1100111: // JALR
        case 0b0001111: // FENCE
        case 0b0000111: // FLW, FLD (Floating-point load)
            imm = (instruction >> 20) & 0xFFF;
            imm = sign_extend(imm, 12);
            break;

        /*** S-TYPE (Store, Floating-Point Store) ***/
        case 0b0100011: // Store (SB, SH, SW, SD)
        case 0b0100111: // FSW, FSD (Floating-point store)
            imm = ((instruction >> 7) & 0x1F) | ((instruction >> 25) & 0x7F) << 5;
            imm = sign_extend(imm, 12);
            break;

        /*** SB-TYPE (branch_ Instructions) ***/
        case 0b1100011: // branch_ (BEQ, BNE, BLT, BGE, BLTU, BGEU)
            imm = ((instruction >> 8) & 0xF) // Bits 11:8
                  | ((instruction >> 25) & 0x3F) << 4 // Bits 10:5
                  | ((instruction >> 7) & 0x1) << 10 // Bit 4
                  | ((instruction >> 31) & 0x1) << 11; // Bit 12
            imm <<= 1;
            imm = sign_extend(imm, 13);
            break;

        /*** U-TYPE (LUI, AUIPC) ***/
        case 0b0110111: // LUI
        case 0b0010111: // AUIPC
            imm = (instruction & 0xFFFFF000) >> 12;  // Upper 20 bits
            
            break;

        /*** J-TYPE (JAL) ***/
        case 0b1101111: // JAL
            imm = ((instruction >> 21) & 0x3FF)  // Bits 10:1
                | ((instruction >> 20) & 0x1) << 10  // Bit 11
                | ((instruction >> 12) & 0xFF) << 11  // Bits 19:12
                | ((instruction >> 31) & 0x1) << 19;  // Bit 20
            imm <<= 1;  // Shift left by 1
            // if (imm & 0x1000) {
            //    imm |= 0xFFFFE000;
            // }
            imm = sign_extend(imm, 21); // Might be 20
            break;

        /*** M-EXTENSION (Multiplication, Division) - R-TYPE ***/
        case 0b0110011: // kMul, kMulh, kMulhu, kMulhsu, kDiv, kDivu, kRem, kRemu
            // R-Type (no immediate needed)
            imm = 0;
            break;

        /*** F-EXTENSION (Floating Point Operations) - R-TYPE ***/
        case 0b1010011: // Floating-point (FADD, FSUB, FMUL, FDIV, FSQRT, etc.)
            // R-Type (no immediate needed)
            imm = 0;
            break;


        default:
            imm = 0;
            break;
    }

    return imm;
}


void VmBase::AddBreakpoint(uint64_t address) {
    breakpoints_.emplace_back(address);
    std::cout << "Breakpoint added at address: " << std::hex << address << std::dec << std::endl;
    std::cout << "Breakpoints: ";
    for (const auto &bp : breakpoints_) {
        std::cout << std::hex << bp << " ";
    }
    std::cout << std::dec << std::endl;
    std::cout << "Program Counter: " << std::hex << program_counter_ << std::dec << std::endl;
}

void VmBase::RemoveBreakpoint(uint64_t address) {
    breakpoints_.erase(std::remove(breakpoints_.begin(), breakpoints_.end(), address), breakpoints_.end());
}

bool VmBase::CheckBreakpoint(uint64_t address) {
    return std::find(breakpoints_.begin(), breakpoints_.end(), address) != breakpoints_.end();
}

void handleSyscall() {
}


