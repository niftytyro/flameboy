#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../addressing/addressing.h"
#include "../utils.h"
#include "arithmetic.h"
#include "registers.h"
#include "utils.h"

void _add_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes, bool should_carry) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

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
  UNUSED(instruction);
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
  uint8_t register_index = extract_register_index(*instruction, 0);

  uint16_t value = read_register_by_name("HL");
  uint16_t operand = read_register(register_index);

  uint16_t result = value + operand;

  write_register_by_name("HL", result / 0x100, result % 0x100);

  write_flags(read_flag('Z'), 0, is_12bit_carry(value, result, 0),
              is_16bit_carry(value, result, 0));

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void cp_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t result = accumulator - operand;

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void cp_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t *operand = read_address(address);
  uint8_t result = accumulator - *operand;

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void cp_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t result = accumulator - operand;

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void dec_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index_r8(*instruction, 0);

  uint8_t value = read_half_register(register_index);
  uint8_t result = value - 1;

  write_half_register(register_index, result);

  write_flags(result == 0, 1, is_4bit_carry(value, result, 1), read_flag('C'));

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void dec_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t result = *value - 1;

  write_address(address, result);

  write_flags(result == 0, 1, is_4bit_carry(*value, result, 1), read_flag('C'));

  *cpu_cycles = 3;
  *number_of_bytes = 1;
}

void dec_r16(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index(*instruction, 0);

  uint8_t value = read_register(register_index);
  uint8_t result = value - 1;

  write_register(register_index, result / 0x100, result % 0x100);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void inc_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index_r8(*instruction, 0);

  uint8_t value = read_half_register(register_index);
  uint8_t result = value + 1;

  write_half_register(register_index, result);

  write_flags(result == 0, 1, is_4bit_carry(value, result, 1), read_flag('C'));

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void inc_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t result = *value + 1;

  write_address(address, result);

  write_flags(result == 0, 1, is_4bit_carry(*value, result, 1), read_flag('C'));

  *cpu_cycles = 3;
  *number_of_bytes = 1;
}

void inc_r16(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index(*instruction, 0);

  uint8_t value = read_register(register_index);
  uint8_t result = value + 1;

  write_register(register_index, result / 0x100, result % 0x100);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void _sub_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes, bool should_carry) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t carry = read_flag('C');

  uint8_t result = accumulator - operand;
  if (should_carry) {
    result -= carry;
  }

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void _sub_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes, bool should_carry) {
  UNUSED(instruction);
  uint16_t address = read_register_by_name("HL");

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t *operand = read_address(address);
  uint8_t carry = read_flag('C');

  uint8_t result = accumulator - *operand;
  if (should_carry) {
    result -= carry;
  }

  write_half_register_by_name('A', result);

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void _sub_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes, bool should_carry) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t carry = read_flag('C');

  uint8_t result = accumulator - operand;
  if (should_carry) {
    result -= carry;
  }

  write_half_register_by_name('A', result);

  write_flags(result == 0, 1, is_4bit_carry(accumulator, result, 1),
              is_8bit_carry(accumulator, result, 1));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void sbc_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _sub_A_n8(instruction, cpu_cycles, number_of_bytes, 1);
}

void sbc_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  _sub_A_HLa(instruction, cpu_cycles, number_of_bytes, 1);
}

void sbc_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _sub_A_n8(instruction, cpu_cycles, number_of_bytes, 1);
}

void sub_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _sub_A_n8(instruction, cpu_cycles, number_of_bytes, 0);
}

void sub_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  _sub_A_HLa(instruction, cpu_cycles, number_of_bytes, 0);
}

void sub_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  _sub_A_n8(instruction, cpu_cycles, number_of_bytes, 0);
}
