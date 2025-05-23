/**
 * @file main_memory.cpp
 * @brief Contains the implementation of the Memory class.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#include "main_memory.h"
#include "../globals.h"

uint8_t Memory::Read(uint64_t address) {
  if (address >= memory_size_) {
    throw std::out_of_range("Memory address out of range: " + std::to_string(address));
  }
  uint64_t block_index = GetBlockIndex(address);
  uint64_t offset = GetBlockOffset(address);
  if (!IsBlockPresent(block_index)) {
    return 0;
  }
  return blocks_[block_index].data[offset];
}

void Memory::Write(uint64_t address, uint8_t value) {
  if (address >= memory_size_) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  uint64_t block_index = GetBlockIndex(address);
  uint64_t offset = GetBlockOffset(address);
  EnsureBlockExists(block_index);
  blocks_[block_index].data[offset] = value;
}

uint64_t Memory::GetBlockIndex(uint64_t address) const {
  return address/block_size_;
}

uint64_t Memory::GetBlockOffset(uint64_t address) const {
  return address%block_size_;
}

bool Memory::IsBlockPresent(uint64_t block_index) const {
  return blocks_.find(block_index)!=blocks_.end();
}

void Memory::EnsureBlockExists(uint64_t block_index) {
  if (blocks_.find(block_index)==blocks_.end()) {
    blocks_.emplace(block_index, MemoryBlock());
  }
}

template<typename T>
T Memory::ReadGeneric(uint64_t address) {
  T value = 0;
  for (size_t i = 0; i < sizeof(T); ++i) {
    value |= static_cast<T>(Read(address + i)) << (8*i);
  }
  return value;
}

template<typename T>
void Memory::WriteGeneric(uint64_t address, T value) {
  for (size_t i = 0; i < sizeof(T); ++i) {
    Write(address + i, static_cast<uint8_t>(value >> (8*i)));
  }
}

uint8_t Memory::ReadByte(uint64_t address) {
  if (address >= memory_size_) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  return Read(address);
}

uint16_t Memory::ReadHalfWord(uint64_t address) {
  if (address >= memory_size_ - 1) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  return ReadGeneric<uint16_t>(address);
}

uint32_t Memory::ReadWord(uint64_t address) {
  if (address >= memory_size_ - 3) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  return ReadGeneric<uint32_t>(address);
}

uint64_t Memory::ReadDoubleWord(uint64_t address) {
  if (address >= memory_size_ - 7) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  return ReadGeneric<uint64_t>(address);
}

float Memory::ReadFloat(uint64_t address) {
  if (address >= memory_size_ - (sizeof(float) - 1)) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));;
  }
  uint32_t value = 0;
  for (size_t i = 0; i < sizeof(float); ++i) {
    value |= static_cast<uint32_t>(Read(address + i)) << (8*i);
  }
  float result;
  std::memcpy(&result, &value, sizeof(float));
  return result;
}

double Memory::ReadDouble(uint64_t address) {
  if (address >= memory_size_ - (sizeof(double) - 1)) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  uint64_t value = 0;
  for (size_t i = 0; i < sizeof(double); ++i) {
    value |= static_cast<uint64_t>(Read(address + i)) << (8*i);
  }
  double result;
  std::memcpy(&result, &value, sizeof(double));
  return result;
}

void Memory::WriteByte(uint64_t address, uint8_t value) {
  if (address >= memory_size_) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  Write(address, value);
}

void Memory::WriteHalfWord(uint64_t address, uint16_t value) {
  if (address >= memory_size_ - 1) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  WriteGeneric<uint16_t>(address, value);
}

void Memory::WriteWord(uint64_t address, uint32_t value) {
  if (address >= memory_size_ - 3) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  WriteGeneric<uint32_t>(address, value);
}

void Memory::WriteDoubleWord(uint64_t address, uint64_t value) {
  if (address >= memory_size_ - 7) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  WriteGeneric<uint64_t>(address, value);
}

void Memory::WriteFloat(uint64_t address, float value) {
  if (address >= memory_size_ - (sizeof(float) - 1)) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  uint32_t value_bits;
  std::memcpy(&value_bits, &value, sizeof(float));
  for (size_t i = 0; i < sizeof(float); ++i) {
    Write(address + i, static_cast<uint8_t>((value_bits >> (8*i)) & 0xFF));
  }
}

void Memory::WriteDouble(uint64_t address, double value) {
  if (address >= memory_size_ - (sizeof(double) - 1)) {
    throw std::out_of_range(std::string("Memory address out of range: ") + std::to_string(address));
  }
  uint64_t value_bits;
  std::memcpy(&value_bits, &value, sizeof(double));
  for (size_t i = 0; i < sizeof(double); ++i) {
    Write(address + i, static_cast<uint8_t>((value_bits >> (8*i)) & 0xFF));
  }
}

void Memory::PrintMemory(const uint64_t address, uint rows) {
  constexpr size_t bytes_per_row = 8; // One row equals 64 bytes
  std::cout << "Memory Dump at Address: 0x" << std::hex << address << std::dec << "\n";
  std::cout << "-----------------------------------------------------------------\n";
  for (uint64_t i = 0; i < rows; ++i) {
    uint64_t current_address = address + (i*bytes_per_row);
    if (current_address >= memory_size_) {
      break;
    }
    std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << current_address << " | ";
    for (size_t j = 0; j < bytes_per_row; ++j) {
      if (current_address + j >= memory_size_) {
        break;
      }
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(Read(current_address + j)) << " ";
    }
    std::cout << "| 0x" << std::hex << std::setw(16) << std::setfill('0')
              << static_cast<int64_t>(ReadDoubleWord(current_address));
    std::cout << std::dec << "\n";
  }
  std::cout << "-----------------------------------------------------------------\n";
}

void dumpMemory(const uint64_t address, uint rows) {
  (void) address;
  (void) rows;
  // TODO: do this
}

void Memory::printMemoryUsage() const {
  std::cout << "Memory Usage Report:\n";
  std::cout << "---------------------\n";
  std::cout << "Block Count: " << blocks_.size() << "\n";
  for (const auto &[block_index, block] : blocks_) {
    size_t used_bytes = std::count_if(block.data.begin(), block.data.end(),
                                      [](uint8_t byte) { return byte!=0; });
    if (used_bytes > 0) {
      std::cout << "Block " << block_index << ": " << used_bytes
                << " / " << block_size_ << " bytes used\n";
    }
  }

}







