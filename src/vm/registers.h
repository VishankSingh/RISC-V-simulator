/**
 * @file registers.h
 * @brief Contains the definition of the RegisterFile class for managing registers in the simulator.
 * @author Vishank Singh, https://github.com/VishankSingh
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include "../pch.h"

/**
 * @brief Represents a register file containing integer, floating-point, and vector registers.
 */
class RegisterFile {
 private:
  static constexpr size_t NUM_GPR = 32; ///< Number of General-Purpose Registers (GPR).
  static constexpr size_t NUM_FPR = 32; ///< Number of Floating-Point Registers (FPR).
  static constexpr size_t NUM_VR = 32;  ///< Number of Vector Registers (VR).

  std::array<uint64_t, NUM_GPR> gpr_ = {}; ///< Array for storing GPR values.
  std::array<uint64_t, NUM_FPR> fpr_ = {}; ///< Array for storing FPR values.
  std::vector<std::array<uint64_t, 8>> vr_ =
      std::vector<std::array<uint64_t, 8>>(NUM_VR); ///< Vector of arrays for VR values.

  static constexpr size_t NUM_CSR = 4096; ///< Number of Control and Status Registers (CSR).

  std::array<uint64_t, NUM_CSR> csr_ = {}; ///< Array for storing CSR values.

  /**
   * @brief Formats the values of registers as a string for display.
   * @tparam T The type of the register values.
   * @param values The array of register values.
   * @return A formatted string representation of the register values.
   */
  template<typename T>
  [[nodiscard]] static std::string FormatRegisterValues(const std::array<T, NUM_GPR> &values);

 public:
  /**
   * @brief Enum representing the type of a register.
   */
  enum class RegisterType {
    INVALID,         ///< Invalid register type.
    INTEGER,         ///< General-purpose integer register.
    FLOATING_POINT,  ///< Floating-point register.
    VECTOR,          ///< Vector register.
    CSR              ///< Control and Status Register (CSR).
  };

  RegisterFile();

  void Reset();

  /**
   * @brief Reads the value of a General-Purpose Register (GPR).
   * @param reg The index of the GPR to read.
   * @return The value of the GPR at the specified index.
   */
  [[nodiscard]] uint64_t ReadGpr(size_t reg) const;

  /**
   * @brief Writes a value to a General-Purpose Register (GPR).
   * @param reg The index of the GPR to write.
   * @param value The value to write.
   */
  void WriteGpr(size_t reg, uint64_t value);

  /**
   * @brief Reads the value of a Floating-Point Register (FPR).
   * @param reg The index of the FPR to read.
   * @return The value of the FPR at the specified index.
   */
  [[nodiscard]] uint64_t ReadFpr(size_t reg) const;

  /**
   * @brief Writes a value to a Floating-Point Register (FPR).
   * @param reg The index of the FPR to write.
   * @param value The value to write.
   */
  void WriteFpr(size_t reg, uint64_t value);

  /**
   * @brief Reads the value of a Vector Register (VR).
   * @param reg The index of the VR to read.
   * @return The array representing the value of the VR at the specified index.
   */
  [[nodiscard]] std::array<uint64_t, 8> ReadVr(size_t reg) const;

  /**
   * @brief Writes a value to a Vector Register (VR).
   * @param reg The index of the VR to write.
   * @param value The array representing the value to write.
   */
  void WriteVr(size_t reg, const std::array<uint64_t, 8> &value);

  [[nodiscard]] uint64_t ReadCsr(size_t reg) const;

  void WriteCsr(size_t reg, uint64_t value);

  /**
   * @brief Retrieves the values of all General-Purpose Registers (GPR).
   * @return A vector containing the values of all GPRs.
   */
  [[nodiscard]] std::vector<uint64_t> GetGprValues() const;

  /**
   * @brief Retrieves the values of all Floating-Point Registers (FPR).
   * @return A vector containing the values of all FPRs.
   */
  [[nodiscard]] std::vector<uint64_t> GetFprValues() const;

  /**
   * @brief Retrieves the values of all Vector Registers (VR).
   * @return A vector containing the arrays of values of all VRs.
   */
  [[nodiscard]] std::vector<std::array<uint64_t, 8>> GetVrValues() const;

  /**
   * @brief Converts the General-Purpose Register (GPR) values to a string.
   * @return A string representation of the GPR values.
   */
  [[nodiscard]] std::string GprToString() const;

  /**
   * @brief Converts the Floating-Point Register (FPR) values to a string.
   * @return A string representation of the FPR values.
   */
  [[nodiscard]] std::string FprToString() const;
  /**
   * @brief Converts the Vector Register (VR) values to a string.
   * @return A string representation of the VR values.
   */
  [[nodiscard]] std::string VrToString() const;

  /**
   * @brief Gets the count of registers of a specific type.
   * @param type The type of register (GPR, FPR, or VR).
   * @return The number of registers of the specified type.
   */
  static size_t GetRegisterCount(RegisterType type);

  /**
   * @brief Gets the name of a General-Purpose Register (GPR).
   * @param reg The index of the GPR.
   * @return The name of the GPR.
   */
  static std::string GetGprName(size_t reg);

  /**
   * @brief Gets the name of a Floating-Point Register (FPR).
   * @param reg The index of the FPR.
   * @return The name of the FPR.
   */
  static std::string GetFprName(size_t reg);

  /**
   * @brief Gets the name of a Vector Register (VR).
   * @param reg The index of the VR.
   * @return The name of the VR.
   */
  static std::string GetVrName(size_t reg);
};

/**
 * @brief Set of valid register names for validation.
 */
extern const std::unordered_set<std::string> valid_registers;

extern const std::unordered_set<std::string> valid_general_purpose_registers;

extern const std::unordered_set<std::string> valid_floating_point_registers;

extern const std::unordered_set<std::string> valid_vector_registers;

extern const std::unordered_set<std::string> valid_csr_registers;

extern const std::unordered_map<std::string, int> csr_to_address;

/**
 * @brief Map of register aliases to their actual names.
 */
extern const std::unordered_map<std::string, std::string> reg_alias_to_name;

bool isValidRegister(const std::string &reg);

bool IsValidGeneralPurposeRegister(const std::string &reg);

bool IsValidFloatingPointRegister(const std::string &reg);

bool IsValidVectorRegister(const std::string &reg);

bool IsValidCsr(const std::string &reg);

#endif // REGISTERS_H