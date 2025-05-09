/**
 * @file rvss_vm.cpp
 * @brief RVSS VM implementation
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "rvss_vm.h"

#include "../../utils.h"
#include "../../globals.h"

RVSSVM::RVSSVM() : VMBase() {
    // Initialize control signals
    dumpRegisters(globals::registers_dump_file, registers_.getGPRValues());
}

RVSSVM::~RVSSVM() = default;


void RVSSVM::fetch() {
    current_instruction_ = memory_controller_.readWord(program_counter_);
    updateProgramCounter(4);
}

void RVSSVM::decode() {
    controlUnit.setControlSignals(current_instruction_);
}

void RVSSVM::execute() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;
    // uint8_t funct7 = (current_instruction_ >> 25) & 0b1111111;
    // uint8_t funct2 = (current_instruction_ >> 25) & 0b11;
    // uint8_t funct6 = (current_instruction_ >> 26) & 0b111111;

    if (opcode == 0x53) { // RV32F & RV64F
        executeFloat();
        return;
    } // TODO: add vector support

    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    // uint8_t rd = (current_instruction_ >> 7) & 0b11111;

    int32_t imm = imm_generator(current_instruction_);

    uint64_t reg1_value = registers_.readGPR(rs1);
    uint64_t reg2_value = registers_.readGPR(rs2);
    
    bool overflow = false;

    
    if (controlUnit.getALUSrc()) {
        reg2_value = imm;
    }


    ALU::ALUOp aluOperation = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    std::tie(execution_result_, overflow) = alu_.execute<int64_t>(aluOperation, reg1_value, reg2_value);

    if (controlUnit.getBranch()) {
        switch (funct3)
        {
        case 0x0: // BEQ
            branch_flag_ = (execution_result_ == 0);
            break;
        case 0x1: // BNE
            branch_flag_ = (execution_result_ != 0);
            break;
        case 0x4: // BLT
            branch_flag_ = (execution_result_ == 1);
            break;
        case 0x5: // BGE
            branch_flag_ = (execution_result_ == 0);
            break;
        case 0x6: // BLTU
            branch_flag_ = (execution_result_ == 1);
            break;
        case 0x7: // BGEU
            branch_flag_ = (execution_result_ == 0);
            break;
        default:
            break;
        }
    }

    if (controlUnit.getBranch() && branch_flag_) { // JAL, JALR
        next_pc_ = static_cast<int64_t>(program_counter_); // PC was already updated in fetch()
        updateProgramCounter(-4);
        updateProgramCounter(imm);
    }

    if (opcode == 0b0010111) { // AUIPC
        execution_result_ = static_cast<int64_t>(program_counter_) - 4 + (imm << 12);

    }
}

void RVSSVM::executeFloat() {
    // if (controlUnit.getALUSrc()) {
    //     freg2 = imm;
    // }

    // ALU::ALUOp aluOp = controlUnit.getALUSignal(current_instruction_, controlUnit.getALUOp());
    // std::tie(fexecution_result_, overflow) = alu_.execute<_Float32>(aluOp, freg1, freg2);
}

void RVSSVM::executeVector() {
    return;
}

void RVSSVM::memory() {
    // uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    // uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    uint8_t funct3 = (current_instruction_ >> 12) & 0b111;

    if (controlUnit.getMemRead()) {
        // registers_.writeGPR(rd, memory_controller_.readWord(execution_result_));
        // switch for ld, lw, lb, lbu, lh, lhu
        switch (funct3) {
        case 0x0: // LB
            memory_result_ = static_cast<int8_t>(memory_controller_.readByte(execution_result_));
            break;
        case 0x1: // LH
            memory_result_ = static_cast<int16_t>(memory_controller_.readHalfWord(execution_result_));
            break;
        case 0x2: // LW
            memory_result_ = static_cast<int32_t>(memory_controller_.readWord(execution_result_));
            break;
        case 0x3: // LD
            memory_result_ = memory_controller_.readDoubleWord(execution_result_);
            break;
        case 0x4: // LBU
            memory_result_ = static_cast<uint8_t>(memory_controller_.readByte(execution_result_));
            break;
        case 0x5: // LHU
            memory_result_ = static_cast<uint16_t>(memory_controller_.readHalfWord(execution_result_));
            break;
        case 0x6: // LWU
            memory_result_ = static_cast<uint32_t>(memory_controller_.readWord(execution_result_));
            break;
        default:
            break;
        }

        // memory_result_ = memory_controller_.readWord(execution_result_);
    } 
    if (controlUnit.getMemWrite()) {
        switch (funct3)
        {
        case 0x0: // SB
            memory_controller_.writeByte(execution_result_, registers_.readGPR(rs2));
            break;
        case 0x1: // SH
            memory_controller_.writeHalfWord(execution_result_, registers_.readGPR(rs2));
            break;
        case 0x2: // SW
            memory_controller_.writeWord(execution_result_, registers_.readGPR(rs2));
            break;
        case 0x3: // SD
            memory_controller_.writeDoubleWord(execution_result_, registers_.readGPR(rs2));
            break;
        default:
            break;
        }
        // memory_controller_.writeWord(execution_result_, registers_.readGPR(rs2));
    }

}

void RVSSVM::writeBack() {
    uint8_t opcode = current_instruction_ & 0b1111111;
    uint8_t rs1 = (current_instruction_ >> 15) & 0b11111;
    // uint8_t rs2 = (current_instruction_ >> 20) & 0b11111;
    uint8_t rd = (current_instruction_ >> 7) & 0b11111;
    int32_t imm = imm_generator(current_instruction_);

    // TODO: Implement floating point support

    if (controlUnit.getRegWrite() && rd != 0) { // Avoid writing to x0
        switch (opcode)
        {
        case 0b0110011: // R-Type
            registers_.writeGPR(rd, execution_result_);
            break;
        case 0b0010011: // I-Type
            registers_.writeGPR(rd, execution_result_);
            break;
        case 0b1100011: // B-Type
            break;
        case 0b0000011: // Load
            registers_.writeGPR(rd, memory_result_);
            break;
        case 0b0100011: // Store
            break;
        case 0b1100111: // JALR
            registers_.writeGPR(rd, next_pc_);
            break;
        case 0b1101111: // JAL
            registers_.writeGPR(rd, next_pc_);
            break;
        case 0b0110111: // LUI
            registers_.writeGPR(rd, (imm << 12));
            break;
        case 0b0010111: // AUIPC
            registers_.writeGPR(rd, execution_result_);
            break;
        default:
            break;
        }
    }

    if (opcode == 0x6F) { // JAL
        // Updated in execute()
    }
    if (opcode == 0x67) { // JALR
        updateProgramCounter((rs1 + imm) & ~1); // Ensure LSB is 0
    }

}




void RVSSVM::run() {
    while(program_counter_ < program_size_) {
        fetch();
        decode();
        execute();
        memory();
        writeBack();
        instructions_retired_++;
        cycle_s++;
    }
    // dumpRegisters(globals::registers_dump_file, registers_.getRegisters());
}

void RVSSVM::debugRun() {
    while (program_counter_ < program_size_) {
        if (std::find(breakpoints_.begin(), breakpoints_.end(), program_counter_) == breakpoints_.end()) {
            fetch();
            decode();
            execute();
            memory();
            writeBack();
            instructions_retired_++;
            cycle_s++;
        } else {
            std::cout << "Breakpoint hit at address: " << program_counter_ << std::endl;
            break;  
        }
    }
}

void RVSSVM::step() {
    fetch();
    decode();
    execute();
    memory();
    writeBack();
    instructions_retired_++;
    cycle_s++;
    dumpRegisters(globals::registers_dump_file, registers_.getGPRValues());
}

void RVSSVM::reset() {
    
}
void RVSSVM::dumpState(const std::string &filename) {
    (void)filename;

}



