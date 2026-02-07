#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "registers.h"
#include "stack.h"
#include "utils.h"

uint8_t get_bit_index(uint8_t *instruction) {
  return BASE_REGISTER_INDEX + (*instruction % 0x40) / 0x8;
}

void bit_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t low = *instruction & 0xf;

  uint8_t bit_index = get_bit_index(instruction);
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;

  uint8_t bit_index = get_bit_index(instruction);
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;

  uint8_t bit_index = get_bit_index(instruction);
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);
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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);
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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);
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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);
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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

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
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

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

void call_n16(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("PC");
  uint16_t target_address = *(instruction + 2) * 0x100 + *(instruction + 1);

  push_to_stack(address);

  write_register_by_name("PC", target_address / 0x100, target_address % 0x100);

  *cpu_cycles = 6;
  *number_of_bytes = 3;
}

void call_NZ_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                 uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (!is_zero) {
    call_n16(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_Z_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (is_zero) {
    call_n16(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_NC_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                 uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (!is_carry) {
    call_n16(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_C_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (is_carry) {
    call_n16(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}
