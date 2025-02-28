/**
 * @file parser.h
 * @brief Contains the definition of the Parser class for parsing tokens and generating intermediate code.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef PARSER_H
#define PARSER_H

#include "../pch.h"

#include "tokens.h"
#include "code_generator.h"
#include "errors.h"

/**
 * @brief Represents a parse error with a specific line number and error message.
 */
struct ParseError {
    unsigned int line; ///< The line number where the error occurred.
    std::string message; ///< The error message.

    /**
     * @brief Constructs a ParseError object.
     * @param line The line number where the error occurred.
     * @param message The error message.
     */
    ParseError(unsigned int line, std::string message) : line(line), message(std::move(message)) {}
};

/**
 * @brief The Parser class is responsible for parsing tokens and generating intermediate code and symbol tables.
 */
class Parser {
private:
    /**
     * @brief Stores data about a symbol.
     */
    struct SymbolData {
        uint64_t address; ///< The address or instruction location of the symbol.
        uint64_t line_number; ///< The line number where the symbol is defined.
        bool isData; ///< Indicates if the symbol represents data or code.
    };

    std::string filename_; ///< The filename being parsed.
    std::vector<Token> tokens_; ///< The list of tokens to parse.
    size_t pos_ = 0; ///< The current position in the token list.
    unsigned int instruction_index_ = 0; ///< The current instruction index.

    /**
     * @brief Tracks parsing errors.
     */
    struct ErrorTracker {
        unsigned int count = 0; ///< The total number of errors.
        std::vector<ParseError> parse_errors; ///< A list of parse errors.
        std::vector<std::variant<
            Errors::SyntaxError,
            Errors::UnexpectedTokenError,
            Errors::ImmediateOutOfRangeError,
            Errors::MisalignedImmediateError,
            Errors::UnexpectedOperandError,
            Errors::InvalidLabelRefError,
            Errors::LabelRedefinitionError,
            Errors::InvalidRegisterError
        >> all_errors; ///< A list of all errors, including syntax and semantic errors.
    };

    ErrorTracker errors_; ///< The error tracker instance.

    std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string>> data_buffer_; ///< The buffer for data directives.

    uint64_t data_index_ = 0; ///< The current index for data allocation.

    std::map<std::string, SymbolData> symbol_table_; ///< The symbol table mapping symbol names to their data.

    std::vector<unsigned int> backPatch; ///< List of instructions requiring backpatching.
    std::vector<std::pair<ICUnit, bool>> IntermediateCode; ///< The generated intermediate code.

    std::map<unsigned int, unsigned int> instruction_number_line_number_mapping; ///< Maps instruction numbers to line numbers.
    std::unordered_map<std::string, unsigned int> symbol_table; ///< The symbol table mapping symbol names to their data.

    /**
     * @brief Returns the previous token in the token list.
     * @return The previous token.
     */
    Token prevToken();

    /**
     * @brief Returns the current token in the token list.
     * @return The current token.
     */
    Token currentToken();

    /**
     * @brief Moves to the next token and returns it.
     * @return The next token.
     */
    Token nextToken();

    /**
     * @brief Peeks ahead by n tokens without advancing the position.
     * @param n The number of tokens to peek ahead.
     * @return The nth token from the current position.
     */
    Token peekToken(int n);

    /**
     * @brief Skips the current line during parsing.
     */
    void skipCurrentLine();

    /**
     * @brief Records a parse error.
     * @param error The parse error to record.
     */
    void recordError(const ParseError &error);

    /**
     * @brief Checks if a register is valid and advances the parser position.
     * @param block The intermediate code unit to update.
     * @return True if the register is valid, false otherwise.
     */
    bool checkAndPassRegister(ICUnit block);

    /**
     * @brief Checks if a general register is valid and advances the parser position.
     * @return True if the general register is valid, false otherwise.
     */
    bool checkGeneralRegister();

    /**
     * @brief Checks if a floating-point register is valid and advances the parser position.
     * @return True if the floating-point register is valid, false otherwise.
     */
    bool checkFloatingPointRegister();

    bool parse_O_GPR_C_GPR_C_GPR();
    bool parse_O_GPR_C_GPR_C_I();
    bool parse_O_GPR_C_I();
    bool parse_O_GPR_C_GPR_C_IL();
    bool parse_O_GPR_C_GPR_C_DL();
    bool parse_O_GPR_C_IL();
    bool parse_O_GPR_C_DL();
    bool parse_O_GPR_C_I_LP_GPR_RP();
    bool parse_O();
    bool parse_pseudo();

    bool parse_O_FPR_C_FPR_C_FPR_C_FPR();
    bool parse_O_FPR_C_FPR_C_FPR();
    bool parse_O_FPR_C_FPR();

    bool parse_O_FPR_C_GPR();
    bool parse_O_GPR_C_FPR();
    bool parse_O_GPR_C_FPR_C_FPR();
    bool parse_O_FPR_C_I_LP_GPR_RP();

    /**
     * @brief Parses a data directive.
     */
    void parseDataDirective();

    /**
     * @brief Parses a text directive.
     */
    void parseTextDirective();

public:
    /**
     * @brief Constructs a Parser instance.
     * @param filename The name of the file to parse.
     * @param tokens The list of tokens to parse.
     */
    Parser(const std::string &filename, const std::vector<Token> &tokens);

    /**
     * @brief Destroys the Parser instance.
     */
    ~Parser();

    /**
     * @brief Parses the tokens to generate intermediate code and symbol tables.
     */
    void parse();

    unsigned int getErrorCount() const;

    /**
     * @brief Returns the list of parse errors.
     * @return A const reference to the vector of parse errors.
     */
    [[nodiscard]] const std::vector<ParseError> &getErrors() const;

    /**
     * @brief Returns the data buffer.
     * @return A reference to the data buffer.
     */
    std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, std::string>> &getDataBuffer();

    /**
     * @brief Returns the generated intermediate code.
     * @return A const reference to the intermediate code vector.
     */
    [[nodiscard]] const std::vector<std::pair<ICUnit, bool>> &getIntermediateCode() const;

    [[nodiscard]] const std::map<unsigned int, unsigned int> &getInstructionNumberLineNumberMapping() const;

    [[nodiscard]] const std::unordered_map<std::string, unsigned int> &getSymbolTable() const;

    /**
     * @brief Prints the list of errors to the console.
     */
    void printErrors() const;

    /**
     * @brief Prints the symbol table to the console.
     */
    void printSymbolTable() const;

    /**
     * @brief Prints the data buffers to the console.
     */
    void printDataBuffers() const;

    /**
     * @brief Prints the intermediate code to the console.
     */
    void printIntermediateCode() const;

};


#endif // PARSER_H
