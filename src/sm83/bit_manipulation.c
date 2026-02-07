#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "../utils.h"
#include "jumps.h"
#include "registers.h"
#include "stack.h"
#include "utils.h"

uint8_t get_bit_index(uint8_t *instruction) {
  return BASE_REGISTER_INDEX + (*instruction % 0x40) / 0x8;
}

void bit_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);

  bool result = (value >> bit_index) % 0x1;

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void bit_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);
  uint16_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);

  bool result = (*value >> bit_index) % 0x1;

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void res_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);

  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);

  bool result = value & ~(0x1 << bit_index);

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void res_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);
  uint16_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);

  bool result = *value & ~(0x1 << bit_index);

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void set_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);

  bool result = value | (0x1 << bit_index);

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void set_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint8_t bit_index = get_bit_index(instruction);
  uint16_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);

  bool result = *value | (0x1 << bit_index);

  write_flags(result, 0, 1, read_flag('c'));

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void rl_r8(uint8_t *instruction, uint8_t *cpu_cycles,
           uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register(register_index);

  bool bit_7 = (0x80 & value) >> 7;
  value = (value << 1) | carry;
  carry = bit_7;

  write_half_register(register_index, value);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void rl_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint16_t value = read_register_by_name("HL");

  bool bit_15 = (0x8000 & value) >> 15;
  value = (value << 1) | carry;
  carry = bit_15;

  write_register_by_name("HL", value / 0x100, value % 0x100);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void rla(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register_by_name('A');

  bool bit_7 = (0x80 & value) >> 7;
  value = (value << 1) | carry;
  carry = bit_7;

  write_half_register_by_name('A', value);
  write_flags(0, 0, 0, carry);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void rlc_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register(register_index);

  bool bit_7 = (0x80 & value) >> 7;
  value = (value << 1) | bit_7;
  carry = bit_7;

  write_half_register(register_index, value);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void rlc_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint16_t value = read_register_by_name("HL");

  bool bit_15 = (0x8000 & value) >> 15;
  value = (value << 1) | bit_15;
  carry = bit_15;

  write_register_by_name("HL", value / 0x100, value % 0x100);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void rlca(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register_by_name('A');

  bool bit_7 = (0x80 & value) >> 7;
  value = (value << 1) | bit_7;
  carry = bit_7;

  write_half_register_by_name('A', value);
  write_flags(0, 0, 0, carry);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void rr_r8(uint8_t *instruction, uint8_t *cpu_cycles,
           uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register(register_index);

  bool bit_0 = 0x1 & value;
  value = (value >> 1) | (carry << 7);
  carry = bit_0;

  write_half_register(register_index, value);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void rr_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint16_t value = read_register_by_name("HL");

  bool bit_0 = 0x1 & value;
  value = (value >> 1) | (carry << 7);
  carry = bit_0;

  write_register_by_name("HL", value / 0x100, value % 0x100);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void rra(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register_by_name('A');

  bool bit_0 = 0x1 & value;
  value = (value >> 1) | (carry << 7);
  carry = bit_0;

  write_half_register_by_name('A', value);
  write_flags(0, 0, 0, carry);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void rrc_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register(register_index);

  bool bit_0 = 0x1 & value;
  value = (value >> 1) | (bit_0 << 7);
  carry = bit_0;

  write_half_register(register_index, value);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void rrc_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint16_t value = read_register_by_name("HL");

  bool bit_0 = (0x1 & value);
  value = (value >> 1) | (bit_0 << 7);
  carry = bit_0;

  write_register_by_name("HL", value / 0x100, value % 0x100);
  write_flags(value == 0, 0, 0, carry);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void rrca(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('c');
  uint8_t value = read_half_register_by_name('A');

  bool bit_0 = (0x1 & value);
  value = (value >> 1) | (bit_0 << 7);
  carry = bit_0;

  write_half_register_by_name('A', value);
  write_flags(0, 0, 0, carry);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void sla_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);
  uint8_t bit_7 = value & 0x80;

  value = value << 1;
  write_half_register(register_index, value);

  write_flags(value, 0, 0, bit_7);
  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void sla_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t bit_7 = *value & 0x80;

  *value = *value << 1;
  write_address(address, *value);

  write_flags(*value, 0, 0, bit_7);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void sra_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);
  uint8_t bit_7 = value & 0x80;
  uint8_t bit_0 = value & 0x01;

  value = (bit_7 * 0x80) + (value >> 1);
  write_half_register(register_index, value);

  write_flags(value, 0, 0, bit_0);
  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void sra_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t bit_7 = *value & 0x80;
  uint8_t bit_0 = *value & 0x01;

  *value = (bit_7 * 0x80) + (*value >> 1);
  write_address(address, *value);

  write_flags(value, 0, 0, bit_0);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void srl_r8(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);
  uint8_t bit_0 = value & 0x01;

  value = (value >> 1);
  write_half_register(register_index, value);

  write_flags(value, 0, 0, bit_0);
  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void srl_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t bit_0 = *value & 0x01;

  *value = (*value >> 1);
  write_address(address, *value);

  write_flags(value, 0, 0, bit_0);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}

void swap_r8(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint8_t value = read_half_register(register_index);
  uint8_t high_v = (value & 0xf0) >> 4;
  uint8_t low_v = value & 0xf;

  value = (low_v << 4) + high_v;
  write_half_register(register_index, value);

  write_flags(value, 0, 0, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void swap_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint8_t address = read_register_by_name("HL");

  uint8_t *value = read_address(address);
  uint8_t high_v = (*value & 0xf0) >> 4;
  uint8_t low_v = *value & 0xf;

  *value = (low_v << 4) + high_v;
  write_address(address, *value);

  write_flags(value, 0, 0, 0);

  *cpu_cycles = 4;
  *number_of_bytes = 2;
}
