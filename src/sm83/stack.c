#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "registers.h"
#include "utils.h"

void push_to_stack(uint16_t value) {
  write_register_by_name("SP", value / 0x100, value ^ 0x100);
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

void dec_SP(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t value = read_register_by_name("SP");
  uint8_t result = value - 1;

  write_register_by_name("SP", result / 0x100, result % 0x100);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void inc_SP(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint8_t value = read_register_by_name("SP");
  uint8_t result = value + 1;

  write_register_by_name("SP", result / 0x100, result % 0x100);

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

void ld_SP_n16(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t byte1 = *(instruction + 1), byte2 = *(instruction + 2);

  write_register_by_name("SP", byte1, byte2);

  *cpu_cycles = 3;
  *number_of_bytes = 3;
}

void ld_HL_SPe8(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  char e8 = *(instruction + 1);
  uint16_t value = read_register_by_name("SP");
  uint16_t new_value = value + e8;

  write_register_by_name("SP", value / 0x100, value % 0x100);

  write_register_by_name("HL", value / 0x100, value % 0x100);

  write_flags(0, 0, is_4bit_carry(value, new_value, e8 < 0),
              is_8bit_carry(value, new_value, e8 < 0));

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ld_SP_HL(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint16_t value = read_register_by_name("HL");

  write_register_by_name("SP", value & 0xff00, value & 0xff00);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_n16a_SP(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t value = read_register_by_name("SP");
  uint16_t address = *(instruction + 1);

  write_address(address, value & 0xff);
  write_address(address + 1, value >> 8);

  *cpu_cycles = 5;
  *number_of_bytes = 3;
}

void pop_AF(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("SP");
  uint8_t *low = read_address(address);
  uint8_t *high = read_address(address + 1);

  write_half_register_by_name('A', *high);
  write_half_register_by_name('F', *low);

  address += 2;
  write_register_by_name("SP", address / 0x100, address % 0x100);

  address++;

  *cpu_cycles = 3;
  *number_of_bytes = 1;
}

void pop_r16(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index(*instruction);
  uint16_t address = read_register_by_name("SP");

  uint8_t *byte1 = read_address(address);
  uint8_t *byte2 = read_address(address + 1);

  address += 2;
  write_register_by_name("SP", address / 0x100, address % 0x100);

  write_register(register_index, *byte1, *byte2);

  *cpu_cycles = 3;
  *number_of_bytes = 1;
}

void push_AF(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("SP");
  uint8_t byte;

  address--;
  byte = read_half_register_by_name('A');
  write_address(address, byte);

  address--;
  byte = read_half_register_by_name('F');
  write_address(address, byte);

  *cpu_cycles = 4;
  *number_of_bytes = 1;
}

void push_r16(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t register_index = extract_register_index(*instruction);
  uint16_t address = read_register_by_name("SP");
  uint16_t value = read_register(register_index);
  uint8_t byte1 = value / 0x100, byte2 = value % 0x100;

  address--;
  write_address(address, byte1);
  address--;
  write_address(address, byte2);

  *cpu_cycles = 4;
  *number_of_bytes = 1;
}
