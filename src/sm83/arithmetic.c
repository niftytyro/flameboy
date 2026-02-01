#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "registers.h"
#include "utils.h"

void _add_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes, bool should_carry) {
  int low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t carry = read_half_register_by_name('C');

  uint8_t result = accumulator + operand;
  if (should_carry) {
    result += carry;
  }

  write_half_register_by_name('A', result);

  write_flags(result == 0, 0, is_4bit_carry(accumulator, result, 0),
              is_8bit_carry(accumulator, result, 0));

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void _add_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes, bool should_carry) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *read_address(read_register_by_name("HL"));
  uint8_t carry = read_half_register_by_name('C');

  uint8_t result = accumulator + operand;
  if (should_carry) {
    result += carry;
  }

  write_half_register_by_name('A', result);

  write_flags(result == 0, 0, is_4bit_carry(accumulator, result, 0),
              is_8bit_carry(accumulator, result, 0));

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void _add_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes, bool should_carry) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t carry = read_half_register_by_name('C');

  uint8_t result = accumulator + operand;
  if (should_carry) {
    result += carry;
  }

  write_half_register_by_name('A', result);

  write_flags(0, 0, is_4bit_carry(accumulator, result, 0),
              is_8bit_carry(accumulator, result, 0));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void adc_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _add_A_r8(instruction, cpu_cycles, number_of_bytes, 1);
}

void adc_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  _add_A_HLa(instruction, cpu_cycles, number_of_bytes, 1);
}

void adc_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _add_A_n8(instruction, cpu_cycles, number_of_bytes, 1);
}

void add_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _add_A_r8(instruction, cpu_cycles, number_of_bytes, 0);
}

void add_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  _add_A_HLa(instruction, cpu_cycles, number_of_bytes, 0);
}

void add_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _add_A_n8(instruction, cpu_cycles, number_of_bytes, 0);
}

void add_HL_r16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;
  uint8_t register_index = extract_register_index(low);

  uint16_t value = read_register_by_name("HL");
  uint16_t operand = read_register(register_index);

  uint16_t result = value + operand;

  write_register_by_name("HL", result / 0x100, result % 0x100);

  write_flags(read_flag('Z'), 0, is_12bit_carry(value, result, 0),
              is_16bit_carry(value, result, 0));

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void add_HL_SP(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t value = read_register_by_name("HL");
  uint16_t operand = read_register_by_name("SP");

  uint16_t result = value + operand;

  write_register_by_name("HL", result / 0x100, result % 0x100);

  write_flags(read_flag('Z'), 0, (result & 0x0f00) > (value & 0x0f00),
              result > value);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void add_SP_e8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  char e8 = *(instruction + 1);
  uint16_t value = read_register_by_name("SP");
  uint16_t result = value + e8;

  write_register_by_name("SP", result / 0x100, result % 0x100);

  write_flags(0, 0, is_4bit_carry(value, result, 0),
              is_8bit_carry(value, result, 0));

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}
