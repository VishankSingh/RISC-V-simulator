/**
 * @file memory_controller.h
 * @brief Contains the declaration of the MemoryController class for managing memory in the VM.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

#include "../pch.h"
#include "../config.h"
#include "main_memory.h"

/**
 * @brief The MemoryController class is responsible for managing memory in the VM.
 */
class MemoryController {
private:
    Memory memory_; ///< The main memory object.
public:
    /**
     * @brief Constructs a MemoryController object.
     */
    MemoryController() = default;

    void Reset() {
        memory_.Reset();
    }

    void PrintCacheStatus() const {

        std::string cacheEnabled = vm_config::ini::Get("Cache", "cache_enabled");
        std::cout << "Cache enabled: " << cacheEnabled << std::endl;
    }

    void WriteByte(uint64_t address, uint8_t value) {
      memory_.WriteByte(address, value);
    }

    void WriteHalfWord(uint64_t address, uint16_t value) {
      memory_.WriteHalfWord(address, value);
    }

    void WriteWord(uint64_t address, uint32_t value) {
      memory_.WriteWord(address, value);
    }

    void WriteDoubleWord(uint64_t address, uint64_t value) {
      memory_.WriteDoubleWord(address, value);
    }

    uint8_t ReadByte(uint64_t address) {
        return memory_.ReadByte(address);
    }

    uint16_t ReadHalfWord(uint64_t address) {
        return memory_.ReadHalfWord(address);
    }

    uint32_t ReadWord(uint64_t address) {
        return memory_.ReadWord(address);

    }

    uint64_t ReadDoubleWord(uint64_t address) {
        return memory_.ReadDoubleWord(address);
    }

    void PrintMemory(const uint64_t address, uint rows) {
      memory_.PrintMemory(address, rows);
    }

    void DumpMemory(const uint64_t address, uint rows) {
      memory_.DumpMemory(address, rows);
    }

};

#endif // MEMORY_CONTROLLER_H

