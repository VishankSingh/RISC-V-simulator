/** @cond DOXYGEN_IGNORE */
/**
 * File Name: instructions.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */
/** @endcond */

#include "instructions.h"

namespace InstructionSet {
static const std::unordered_set<std::string> valid_instructions = {
        "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu",
        "addw", "subw", "sllw", "srlw", "sraw",
        "addi", "xori", "ori", "andi", "slli", "srli", "srai", "slti", "sltiu",
        "addiw", "slliw", "srliw", "sraiw",
        "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
        "sb", "sh", "sw", "sd",
        "beq", "bne", "blt", "bge", "bltu", "bgeu",
        "lui", "auipc",
        "jal", "jalr",
        "ecall", "ebreak",

        "la", "nop", "li", "mv", "not", "neg", "negw", 
        "sext.w", "seqz", "snez", "sltz", "sgtz",
        "beqz", "bnez", "blez", "bgez", "bltz", "bgtz",
        "bgt", "ble", "bgtu", "bleu",
        "j", "jr", "ret", "call", "tail", "fence", "fence_i",


        "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",
        "mulw", "divw", "divuw", "remw", "remuw",


        "flw", "fsw", "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
        "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
        "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
        "fmin.s", "fmax.s",
        "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
        "feq.s", "flt.s", "fle.s",
        "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
        "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",

};

static const std::unordered_set<std::string> RTypeInstructions = {
        // Base RV32I
        "add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu",

        // RV64
        "addw", "subw", "sllw", "srlw", "sraw",

        // M Extension
        "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",

        // M Extension RV64
        "mulw", "divw", "divuw", "remw", "remuw",

        // F Extension
        "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
        "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
        "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
        "fmin.s", "fmax.s",
        "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
        "feq.s", "flt.s", "fle.s",
        "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
        "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",

};

static const std::unordered_set<std::string> ITypeInstructions = {
        "addi", "xori", "ori", "andi", "slli", "srli", "srai", "slti", "sltiu",
        "addiw", "slliw", "srliw", "sraiw",
        "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
        "jalr"
};

static const std::unordered_set<std::string> I1TypeInstructions = {
        "addi", "xori", "ori", "andi", "sltiu", "slti",
        "addiw",
        "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu",
        "jalr"
};

static const std::unordered_set<std::string> I2TypeInstructions = {
        "slli", "srli", "srai",
        "slliw", "srliw", "sraiw"
};

static const std::unordered_set<std::string> I3TypeInstructions = {
        "ecall", "ebreak"
};

static const std::unordered_set<std::string> STypeInstructions = {
        "sb", "sh", "sw", "sd"
};

static const std::unordered_set<std::string> BTypeInstructions = {
        "beq", "bne", "blt", "bge", "bltu", "bgeu"
};

static const std::unordered_set<std::string> UTypeInstructions = {
        "lui", "auipc"
};

static const std::unordered_set<std::string> JTypeInstructions = {
        "jal"
};

static const std::unordered_set<std::string> PseudoInstructions = {
        "la", "nop", "li", "mv", "not", "neg", "negw", 
        "sext.w", "seqz", "snez", "sltz", "sgtz",
        "beqz", "bnez", "blez", "bgez", "bltz", "bgtz",
        "bgt", "ble", "bgtu", "bleu",
        "j", "jr", "ret", "call", "tail", "fence", "fence_i",
};

static const std::unordered_set<std::string> MExtInstructions = {
        "mul", "mulh", "mulhsu", "mulhu", "div", "divu", "rem", "remu",
        "mulw", "divw", "divuw", "remw", "remuw"
};

static const std::unordered_set<std::string> FExtensionInstructions = {
        "flw", "fsw", "fmadd.s", "fmsub.d", "fnmsub.s", "fnmadd.s",
        "fadd.s", "fsub.s", "fmul.s", "fdiv.s", "fsqrt.s",
        "fsgnj.s", "fsgnjn.s", "fsgnjx.s",
        "fmin.s", "fmax.s",
        "fcvt.w.s", "fcvt.wu.s", "fmv.x.w",
        "feq.s", "flt.s", "fle.s",
        "fclass.s", "fcvt.s.w", "fcvt.s.wu", "fmv.w.x",
        "fcvt.l.s", "fcvt.lu.s", "fcvt.s.l", "fcvt.s.lu",
};


std::unordered_map<std::string, RTypeInstructionEncoding> R_type_instruction_encoding_map = {
        {"add",  {0b0110011, 0b000, 0b0000000}}, // O_R_C_R_C_R
        {"sub",  {0b0110011, 0b000, 0b0100000}}, // O_R_C_R_C_R
        {"xor",  {0b0110011, 0b100, 0b0000000}}, // O_R_C_R_C_R
        {"or",   {0b0110011, 0b110, 0b0000000}}, // O_R_C_R_C_R
        {"and",  {0b0110011, 0b111, 0b0000000}}, // O_R_C_R_C_R
        {"sll",  {0b0110011, 0b001, 0b0000000}}, // O_R_C_R_C_R
        {"srl",  {0b0110011, 0b101, 0b0000000}}, // O_R_C_R_C_R
        {"sra",  {0b0110011, 0b101, 0b0100000}}, // O_R_C_R_C_R
        {"slt",  {0b0110011, 0b010, 0b0000000}}, // O_R_C_R_C_R
        {"sltu", {0b0110011, 0b011, 0b0000000}}, // O_R_C_R_C_R

        {"addw", {0b0111011, 0b000, 0b0000000}}, // O_R_C_R_C_R
        {"subw", {0b0111011, 0b000, 0b0100000}}, // O_R_C_R_C_R
        {"sllw", {0b0111011, 0b001, 0b0000000}}, // O_R_C_R_C_R
        {"srlw", {0b0111011, 0b101, 0b0000000}}, // O_R_C_R_C_R
        {"sraw", {0b0111011, 0b101, 0b0100000}}, // O_R_C_R_C_R
};

std::unordered_map<std::string, I1TypeInstructionEncoding> I1_type_instruction_encoding_map = {
        {"addi",  {0b0010011, 0b000}}, // O_R_C_R_C_I
        {"xori",  {0b0010011, 0b100}}, // O_R_C_R_C_I
        {"ori",   {0b0010011, 0b110}}, // O_R_C_R_C_I
        {"andi",  {0b0010011, 0b111}}, // O_R_C_R_C_I
        {"sltiu", {0b0010011, 0b011}}, // O_R_C_R_C_I
        {"slti",  {0b0010011, 0b010}}, // O_R_C_R_C_I

        {"addiw", {0b0011011, 0b000}}, // O_R_C_R_C_I

        {"lh",    {0b0000011, 0b001}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"lw",    {0b0000011, 0b010}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"lb",    {0b0000011, 0b000}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"ld",    {0b0000011, 0b011}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"lbu",   {0b0000011, 0b100}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"lhu",   {0b0000011, 0b101}}, // O_R_C_I_LP_R_RP, O_R_C_DL
        {"lwu",   {0b0000011, 0b110}}, // O_R_C_I_LP_R_RP, O_R_C_DL

        {"jalr",  {0b1100111, 0b000}}, // O_R_C_I, O_R_C_IL
};

std::unordered_map<std::string, I3TypeInstructionEncoding> I3_type_instruction_encoding_map = {
        {"ecall",  {0b1110011, 0b000, 0b0000000}}, // O
        {"ebreak", {0b1110011, 0b001, 0b0000000}}, // O
};

std::unordered_map<std::string, I2TypeInstructionEncoding> I2_type_instruction_encoding_map = {
        {"slli",  {0b0010011, 0b001, 0b000000}}, // O_R_C_R_C_I
        {"srli",  {0b0010011, 0b101, 0b000000}}, // O_R_C_R_C_I
        {"srai",  {0b0010011, 0b101, 0b010000}}, // O_R_C_R_C_I

        {"slliw", {0b0011011, 0b001, 0b000000}}, // O_R_C_R_C_I
        {"srliw", {0b0011011, 0b101, 0b000000}}, // O_R_C_R_C_I
        {"sraiw", {0b0011011, 0b101, 0b010000}}, // O_R_C_R_C_I
};

std::unordered_map<std::string, STypeInstructionEncoding> S_type_instruction_encoding_map = {
        {"sb", {0b0100011, 0b000}}, // O_R_C_R_C_I
        {"sh", {0b0100011, 0b001}}, // O_R_C_R_C_I
        {"sw", {0b0100011, 0b010}}, // O_R_C_R_C_I
        {"sd", {0b0100011, 0b011}}, // O_R_C_R_C_I
};

std::unordered_map<std::string, BTypeInstructionEncoding> B_type_instruction_encoding_map = {
        {"beq",  {0b1100011, 0b000}}, // O_R_C_R_C_I, O_R_C_R_C_IL
        {"bne",  {0b1100011, 0b001}}, // O_R_C_R_C_I, O_R_C_R_C_IL
        {"blt",  {0b1100011, 0b100}}, // O_R_C_R_C_I, O_R_C_R_C_IL
        {"bge",  {0b1100011, 0b101}}, // O_R_C_R_C_I, O_R_C_R_C_IL
        {"bltu", {0b1100011, 0b110}}, // O_R_C_R_C_I, O_R_C_R_C_IL
        {"bgeu", {0b1100011, 0b111}}, // O_R_C_R_C_I, O_R_C_R_C_IL
};

std::unordered_map<std::string, UTypeInstructionEncoding> U_type_instruction_encoding_map = {
        {"lui",   {0b0110111}}, // O_R_C_I
        {"auipc", {0b0010111}}, // O_R_C_I
};

std::unordered_map<std::string, JTypeInstructionEncoding> J_type_instruction_encoding_map = {
        {"jal", {0b1101111}}, // O_R_C_IL
};


std::unordered_map<std::string, InstructionType> instruction_opcode_type_map = {
        {"add",         InstructionType::R},
        {"sub",         InstructionType::R},
        {"xor",         InstructionType::R},
        {"or",          InstructionType::R},
        {"and",         InstructionType::R},
        {"sll",         InstructionType::R},
        {"srl",         InstructionType::R},
        {"sra",         InstructionType::R},
        {"slt",         InstructionType::R},
        {"sltu",        InstructionType::R},

        {"addw",        InstructionType::R},
        {"subw",        InstructionType::R},
        {"sllw",        InstructionType::R},
        {"srlw",        InstructionType::R},
        {"sraw",        InstructionType::R},

        {"addi",        InstructionType::I1},
        {"xori",        InstructionType::I1},
        {"ori",         InstructionType::I1},
        {"andi",        InstructionType::I1},
        {"slli",        InstructionType::I2},
        {"srli",        InstructionType::I2},
        {"srai",        InstructionType::I2},
        {"slti",        InstructionType::I1},
        {"sltiu",       InstructionType::I1},

        // {"addiw",     InstructionType::I1},
        // {"slliw",     InstructionType::I2},
        // {"srliw",     InstructionType::I2},
        // {"sraiw",     InstructionType::I2},

        {"lb",          InstructionType::I1},
        {"lh",          InstructionType::I1},
        {"lw",          InstructionType::I1},
        {"ld",          InstructionType::I1},
        {"lbu",         InstructionType::I1},
        {"lhu",         InstructionType::I1},
        {"lwu",         InstructionType::I1},

        {"jalr",        InstructionType::I1},

        {"sb",          InstructionType::S},
        {"sh",          InstructionType::S},
        {"sw",          InstructionType::S},
        {"sd",          InstructionType::S},

        {"beq",         InstructionType::B},
        {"bne",         InstructionType::B},
        {"blt",         InstructionType::B},
        {"bge",         InstructionType::B},
        {"bltu",        InstructionType::B},
        {"bgeu",        InstructionType::B},

        {"lui",         InstructionType::U},
        {"auipc",       InstructionType::U},

        {"jal",         InstructionType::J},

        {"ecall",       InstructionType::I3},
        {"ebreak",      InstructionType::I3},
};

std::unordered_map<std::string, InstructionEncoding> instruction_encoding_map = {
//==RV64M======================================================================================
        // R-Type
        {"mul",       {"0110011", "000", "0000001", ""}}, // O_R_C_R_C_R
        {"mulh",      {"0110011", "001", "0000001", ""}}, // O_R_C_R_C_R
        {"mulhsu",    {"0110011", "010", "0000001", ""}}, // O_R_C_R_C_R
        {"mulhu",     {"0110011", "011", "0000001", ""}}, // O_R_C_R_C_R
        {"div",       {"0110011", "100", "0000001", ""}}, // O_R_C_R_C_R
        {"divu",      {"0110011", "101", "0000001", ""}}, // O_R_C_R_C_R
        {"rem",       {"0110011", "110", "0000001", ""}}, // O_R_C_R_C_R
        {"remu",      {"0110011", "111", "0000001", ""}}, // O_R_C_R_C_R

        {"mulw",      {"0111011", "000", "0000001", ""}}, // O_R_C_R_C_R
        {"divw",      {"0111011", "100", "0000001", ""}}, // O_R_C_R_C_R
        {"divuw",     {"0111011", "101", "0000001", ""}}, // O_R_C_R_C_R
        {"remw",      {"0111011", "110", "0000001", ""}}, // O_R_C_R_C_R
        {"remuw",     {"0111011", "111", "0000001", ""}}, // O_R_C_R_C_R

//==RV64F======================================================================================
        // I-Type
        {"flw",       {"0000111", "010", "",        ""}}, // O_R_C_I_LP_R_RP

        // S-Type
        {"fsw",       {"0100111", "010", "",        ""}}, // O_R_C_I_LP_R_RP


        {"fadd.s",    {"1010011", "000", "0000000", ""}}, // O_R_C_R_C_R
        {"fsub.s",    {"1010011", "000", "0000100", ""}}, // O_R_C_R_C_R
        {"fmul.s",    {"1010011", "000", "0001000", ""}}, // O_R_C_R_C_R
        {"fdiv.s",    {"1010011", "000", "0001100", ""}}, // O_R_C_R_C_R
        {"fsqrt.s",   {"1010011", "010", "0101100", ""}}, // O_R_C_R_C_R
        {"fsgnj.s",   {"1010011", "000", "0010000", ""}}, // O_R_C_R_C_R
        {"fsgnjn.s",  {"1010011", "001", "0010000", ""}}, // O_R_C_R_C_R
        {"fsgnjx.s",  {"1010011", "010", "0010000", ""}}, // O_R_C_R_C_R
        {"fmin.s",    {"1010011", "000", "0001100", ""}}, // O_R_C_R_C_R
        {"fmax.s",    {"1010011", "001", "0001100", ""}}, // O_R_C_R_C_R
        {"feq.s",     {"1010011", "101", "1010000", ""}}, // O_R_C_R_C_R
        {"flt.s",     {"1010011", "110", "1010000", ""}}, // O_R_C_R_C_R
        {"fle.s",     {"1010011", "111", "1010000", ""}}, // O_R_C_R_C_R

        {"fcvt.w.s",  {"1010011", "000", "1100000", ""}}, // O_R_C_R_C_R
        {"fcvt.wu.s", {"1010011", "000", "1100001", ""}}, // O_R_C_R_C_R
        {"fcvt.s.w",  {"1010011", "001", "1100000", ""}}, // O_R_C_R_C_R
        {"fcvt.s.wu", {"1010011", "001", "1100001", ""}}, // O_R_C_R_C_R

        {"fcvt.l.s",  {"1010011", "010", "1100000", ""}}, // O_R_C_R_C_R
        {"fcvt.lu.s", {"1010011", "011", "1100000", ""}}, // O_R_C_R_C_R
        {"fcvt.s.l",  {"1010011", "101", "1100000", ""}}, // O_R_C_R_C_R
        {"fcvt.s.lu", {"1010011", "110", "1100000", ""}}, // O_R_C_R_C_R

        {"fclass.s",  {"1010011", "000", "1110000", ""}}, // O_R_C_R_C_R

        {"fmadd.s",   {"1000011", "00",  "0000000", ""}}, // O_R_C_R_C_R
        {"fmsub.s",   {"1000111", "00",  "0000000", ""}}, // O_R_C_R_C_R
        {"fnmsub.s",  {"1001011", "00",  "0000000", ""}}, // O_R_C_R_C_R
        {"fnmadd.s",  {"1001111", "00",  "0000000", ""}}, // O_R_C_R_C_R
        {"fmv.x.w",   {"1010011", "000", "1110000", ""}}, // O_R_C_R_C_R
        {"fmv.w.x",   {"1010011", "000", "1110000", ""}}, // O_R_C_R_C_R


//==RV64D======================================================================================

        {"fadd.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"fsub.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmul.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"fdiv.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"fsqrt.d",   {"", "", "", ""}}, // O_R_C_R_C_R
        {"fsgnj.d",   {"", "", "", ""}}, // O_R_C_R_C_R
        {"fsgnjn.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fsgnjx.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmin.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmax.d",    {"", "", "", ""}}, // O_R_C_R_C_R
        {"feq.d",     {"", "", "", ""}}, // O_R_C_R_C_R
        {"flt.d",     {"", "", "", ""}}, // O_R_C_R_C_R
        {"fle.d",     {"", "", "", ""}}, // O_R_C_R_C_R

        {"fcvt.w.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.wu.d", {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.d.w",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.d.wu", {"", "", "", ""}}, // O_R_C_R_C_R

        {"fcvt.l.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.lu.d", {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.d.l",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.d.lu", {"", "", "", ""}}, // O_R_C_R_C_R

        {"fld",       {"", "",  "", ""}}, // O_R_C_R_C_R
        {"fsd",       {"", "",  "", ""}}, // O_R_C_R_C_R

        {"fcvt.s.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fcvt.d.s",  {"", "", "", ""}}, // O_R_C_R_C_R

        {"fclass.d",  {"", "", "", ""}}, // O_R_C_R_C_R

        {"fmadd.d",   {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmsub.d",   {"", "", "", ""}}, // O_R_C_R_C_R
        {"fnmsub.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fnmadd.d",  {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmv.x.d",   {"", "", "", ""}}, // O_R_C_R_C_R
        {"fmv.d.x",   {"", "", "", ""}}, // O_R_C_R_C_R

        {"fld",       {"", "",  "", ""}}, // O_R_C_R_C_R
        {"fsd",       {"", "",  "", ""}}, // O_R_C_R_C_R
        




};


/*
    O_R_C_R_C_R: <opcode><register><comma><register><comma><register>
    O_R_C_R_C_I: <opcode><register><comma><register><comma><immediate>
    O_R_C_R_C_IL: <opcode><register><comma><register><comma><instruction_label>
    O_R_C_R_C_DL: <opcode><register><comma><register><comma><data_label>
    O_R_C_I_LP_R_RP: <opcode><register><comma><immediate><lparen><register><rparen>
    O_R_C_I: <opcode><register><comma><immediate>
    O_R_C_IL: <opcode><register><comma><instruction_label>
    O_R_C_DL: <opcode><register><comma><data_label>
    O: <opcode>

    DL -> Data Label
    IL -> Instruction Label
    

*/
std::unordered_map<std::string, std::vector<SyntaxType>> instruction_syntax_map = {
        {"add",       {SyntaxType::O_R_C_R_C_R}},
        {"sub",       {SyntaxType::O_R_C_R_C_R}},
        {"xor",       {SyntaxType::O_R_C_R_C_R}},
        {"or",        {SyntaxType::O_R_C_R_C_R}},
        {"and",       {SyntaxType::O_R_C_R_C_R}},
        {"sll",       {SyntaxType::O_R_C_R_C_R}},
        {"srl",       {SyntaxType::O_R_C_R_C_R}},
        {"sra",       {SyntaxType::O_R_C_R_C_R}},
        {"slt",       {SyntaxType::O_R_C_R_C_R}},
        {"sltu",      {SyntaxType::O_R_C_R_C_R}},

        {"addi",      {SyntaxType::O_R_C_R_C_I}},
        {"xori",      {SyntaxType::O_R_C_R_C_I}},
        {"ori",       {SyntaxType::O_R_C_R_C_I}},
        {"andi",      {SyntaxType::O_R_C_R_C_I}},
        {"slli",      {SyntaxType::O_R_C_R_C_I}},
        {"srli",      {SyntaxType::O_R_C_R_C_I}},
        {"srai",      {SyntaxType::O_R_C_R_C_I}},
        {"slti",      {SyntaxType::O_R_C_R_C_I}},
        {"sltiu",     {SyntaxType::O_R_C_R_C_I}},

        {"lb",        {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"lh",        {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"lw",        {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"ld",        {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"lbu",       {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"lhu",       {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},
        {"lwu",       {SyntaxType::O_R_C_I_LP_R_RP, SyntaxType::O_R_C_DL}},

        {"sb",        {SyntaxType::O_R_C_I_LP_R_RP}},
        {"sh",        {SyntaxType::O_R_C_I_LP_R_RP}},
        {"sw",        {SyntaxType::O_R_C_I_LP_R_RP}},
        {"sd",        {SyntaxType::O_R_C_I_LP_R_RP}},

        {"beq",       {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},
        {"bne",       {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},
        {"blt",       {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},
        {"bge",       {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},
        {"bltu",      {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},
        {"bgeu",      {SyntaxType::O_R_C_R_C_I,     SyntaxType::O_R_C_R_C_IL}},

        {"lui",       {SyntaxType::O_R_C_I}},
        {"auipc",     {SyntaxType::O_R_C_I}},

        {"jal",       {SyntaxType::O_R_C_I,         SyntaxType::O_R_C_IL}},

        {"jalr",      {SyntaxType::O_R_C_I_LP_R_RP}},

        {"ecall",     {SyntaxType::O}},
        {"ebreak",    {SyntaxType::O}},

//////////////////////////////////////////////////////////////////////////////

        {"nop",       {SyntaxType::PSEUDO}},
        {"li",        {SyntaxType::PSEUDO}},
        {"mv",        {SyntaxType::PSEUDO}},
        {"not",       {SyntaxType::PSEUDO}},
        {"neg",       {SyntaxType::PSEUDO}},
        {"negw",      {SyntaxType::PSEUDO}},
        {"sext.w",    {SyntaxType::PSEUDO}},
        {"seqz",      {SyntaxType::PSEUDO}},
        {"snez",      {SyntaxType::PSEUDO}},
        {"sltz",      {SyntaxType::PSEUDO}},
        {"sgtz",      {SyntaxType::PSEUDO}},
        {"beqz",      {SyntaxType::PSEUDO}},
        {"bnez",      {SyntaxType::PSEUDO}},
        {"blez",      {SyntaxType::PSEUDO}},
        {"bgez",      {SyntaxType::PSEUDO}},
        {"bltz",      {SyntaxType::PSEUDO}},
        {"bgtz",      {SyntaxType::PSEUDO}},
        {"bgt",       {SyntaxType::PSEUDO}},
        {"ble",       {SyntaxType::PSEUDO}},
        {"bgtu",      {SyntaxType::PSEUDO}},
        {"bleu",      {SyntaxType::PSEUDO}},
        {"j",         {SyntaxType::PSEUDO}},
        {"jr",        {SyntaxType::PSEUDO}},
        {"ret",       {SyntaxType::PSEUDO}},
        {"call",      {SyntaxType::PSEUDO}},
        {"tail",      {SyntaxType::PSEUDO}},
        {"fence",     {SyntaxType::PSEUDO}},
        {"fence_i",   {SyntaxType::PSEUDO}},

//////////////////////////////////////////////////////////////////////////////
        {"mul",       {SyntaxType::O_R_C_R_C_R}},
        {"mulh",      {SyntaxType::O_R_C_R_C_R}},
        {"mulhsu",    {SyntaxType::O_R_C_R_C_R}},
        {"mulhu",     {SyntaxType::O_R_C_R_C_R}},
        {"div",       {SyntaxType::O_R_C_R_C_R}},
        {"divu",      {SyntaxType::O_R_C_R_C_R}},
        {"rem",       {SyntaxType::O_R_C_R_C_R}},
        {"remu",      {SyntaxType::O_R_C_R_C_R}},

        {"mulw",      {SyntaxType::O_R_C_R_C_R}},
        {"divw",      {SyntaxType::O_R_C_R_C_R}},
        {"divuw",     {SyntaxType::O_R_C_R_C_R}},
        {"remw",      {SyntaxType::O_R_C_R_C_R}},
        {"remuw",     {SyntaxType::O_R_C_R_C_R}},


        // TODO: Check if these are correct
        {"flw",       {SyntaxType::O_R_C_I_LP_R_RP}},
        {"fsw",       {SyntaxType::O_R_C_I_LP_R_RP}},

        {"fmadd.s",   {SyntaxType::O_R_C_R_C_R}},
        {"fmsub.d",   {SyntaxType::O_R_C_R_C_R}},
        {"fnmsub.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fnmadd.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fadd.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fsub.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fmul.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fdiv.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fsqrt.s",   {SyntaxType::O_R_C_R_C_R}},
        {"fsgnj.s",   {SyntaxType::O_R_C_R_C_R}},
        {"fsgnjn.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fsgnjx.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fmin.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fmax.s",    {SyntaxType::O_R_C_R_C_R}},
        {"fcvt.w.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fcvt.wu.s", {SyntaxType::O_R_C_R_C_R}},
        {"fmv.x.w",   {SyntaxType::O_R_C_R_C_R}},
        {"feq.s",     {SyntaxType::O_R_C_R_C_R}},
        {"flt.s",     {SyntaxType::O_R_C_R_C_R}},
        {"fle.s",     {SyntaxType::O_R_C_R_C_R}},
        {"fclass.s",  {SyntaxType::O_R_C_R_C_R}},
        {"fcvt.s.w",  {SyntaxType::O_R_C_R_C_R}},
        {"fcvt.s.wu", {SyntaxType::O_R_C_R_C_R}},
        {"fmv.w.x",   {SyntaxType::O_R_C_R_C_R}},
};


bool isValidInstruction(const std::string &instruction) {
    return valid_instructions.find(instruction) != valid_instructions.end();
}


bool isValidRTypeInstruction(const std::string &instruction) {
    return RTypeInstructions.find(instruction) != RTypeInstructions.end();
}

bool isValidITypeInstruction(const std::string &instruction) {
    return (I1TypeInstructions.find(instruction) != I1TypeInstructions.end()) ||
           (I2TypeInstructions.find(instruction) != I2TypeInstructions.end()) ||
           (I3TypeInstructions.find(instruction) != I3TypeInstructions.end());
}

bool isValidI1TypeInstruction(const std::string &instruction) {
    return I1TypeInstructions.find(instruction) != I1TypeInstructions.end();
}

bool isValidI2TypeInstruction(const std::string &instruction) {
    return I2TypeInstructions.find(instruction) != I2TypeInstructions.end();
}

bool isValidI3TypeInstruction(const std::string &instruction) {
    return I3TypeInstructions.find(instruction) != I3TypeInstructions.end();
}

bool isValidSTypeInstruction(const std::string &instruction) {
    return STypeInstructions.find(instruction) != STypeInstructions.end();
}

bool isValidBTypeInstruction(const std::string &instruction) {
    return BTypeInstructions.find(instruction) != BTypeInstructions.end();
}

bool isValidUTypeInstruction(const std::string &instruction) {
    return UTypeInstructions.find(instruction) != UTypeInstructions.end();
}

bool isValidJTypeInstruction(const std::string &instruction) {
    return JTypeInstructions.find(instruction) != JTypeInstructions.end();
}

bool isValidPseudoInstruction(const std::string &instruction) {
    return PseudoInstructions.find(instruction) != PseudoInstructions.end();
}

std::string getExpectedSyntaxes(const std::string &opcode) {
    static const std::unordered_map<std::string, std::string> opcodeSyntaxMap = {
        {"nop", "nop"},
        {"li", "li <reg>, <imm>"},
        {"mv", "mv <reg>, <reg>"},
        {"not", "not <reg>, <reg>"},
        {"neg", "neg <reg>, <reg>"},
        {"seqz", "seqz <reg>, <reg>"},
        {"snez", "snez <reg>, <reg>"},
        {"sltz", "sltz <reg>, <reg>"},
        {"sgtz", "sgtz <reg>, <reg>"},
        {"beqz", "beqz <reg>, <text label>"},
        {"bnez", "bnez <reg>, <text label>"},
        {"blez", "blez <reg>, <text label>"},
        {"bgez", "bgez <reg>, <text label>"},
        {"bltz", "bltz <reg>, <text label>"},
        {"bgtz", "bgtz <reg>, <text label>"},
        {"j", "j <text label>"},
        {"jal", "jal <text label>"},
        {"jr", "jr <reg>"},
        {"jalr", "jalr <reg>"},
        {"la", "la <reg>, <text label>"},
        {"call", "call <text label>"},
        {"tail", "tail <text label>"},
        {"fence", "fence"}
    };

    auto opcodeIt = opcodeSyntaxMap.find(opcode);
    if (opcodeIt != opcodeSyntaxMap.end()) {
        return opcodeIt->second;
    }

    static const std::unordered_map<SyntaxType, std::string> syntaxTypeToString = {
        {SyntaxType::O, "<empty>"},
        {SyntaxType::O_R_C_R_C_R, "<reg>, <reg>, <reg>"},
        {SyntaxType::O_R_C_R_C_I, "<reg>, <reg>, <imm>"},
        {SyntaxType::O_R_C_R_C_IL, "<reg>, <reg>, <text label>"},
        {SyntaxType::O_R_C_R_C_DL, "<reg>, <reg>, <data label>"},
        {SyntaxType::O_R_C_I_LP_R_RP, "<reg>, <imm>(<reg>)"},
        {SyntaxType::O_R_C_I, "<reg>, <imm>"},
        {SyntaxType::O_R_C_IL, "<reg>, <text label>"},
        {SyntaxType::O_R_C_DL, "<reg>, <data label>"}
    };

    std::string syntaxes;
    const auto &syntaxList = instruction_syntax_map[opcode];
    for (size_t i = 0; i < syntaxList.size(); ++i) {
        if (i > 0) {
            syntaxes += " or ";
        }
        auto syntaxIt = syntaxTypeToString.find(syntaxList[i]);
        if (syntaxIt != syntaxTypeToString.end()) {
            syntaxes += opcode + " " + syntaxIt->second;
        }
    }

    return syntaxes;
}

} // namespace InstructionSet