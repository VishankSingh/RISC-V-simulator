#include "pch.h"


#include "main.h"
#include "./assembler/tokens.h"
#include "./assembler/lexer.h"
#include "./assembler/parser.h"
#include "./assembler/code_generator.h"

#include "./assembler/assembler.h"

#include "./vm/memory_controller.h"

int main() {
    AssembledProgram program;
    try {
        program = assemble("/home/vis/Desk/assembler/examples/e1.s");
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 0;
    }

    // std::cout << "Program: " << program.filename << std::endl;
    // for (const std::bitset<32> &instruction: program.instruction_buffer) {
    //     std::cout << instruction << std::endl;
    // }

    // std::cout << "globals::config_file: " << globals::config_file << std::endl;


    // MemoryController memoryController;
    // memoryController.printCacheStatus();



    return 0;
}