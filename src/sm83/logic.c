#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "registers.h"
#include "utils.h"

void and_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t result = accumulator & operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 1, 0);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void and_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t address = read_register_by_name("HL");

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t *operand = read_address(address);
  uint8_t result = accumulator & *operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 1, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void and_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t result = accumulator & operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 1, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void cpl(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');

  write_half_register_by_name('A', ~accumulator);
  write_flags(read_flag('z'), 1, 1, read_flag('c'));

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void or_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t result = accumulator | operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void or_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t address = read_register_by_name("HL");

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t *operand = read_address(address);
  uint8_t result = accumulator | *operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void or_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t result = accumulator | operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void xor_A_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t low = *instruction & 0xf;
  uint8_t register_index = extract_half_register_index(low);

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = read_half_register(register_index);
  uint8_t result = accumulator ^ operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void xor_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint8_t address = read_register_by_name("HL");

  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t *operand = read_address(address);
  uint8_t result = accumulator ^ *operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void xor_A_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t operand = *(instruction + 1);
  uint8_t result = accumulator ^ operand;

  write_half_register_by_name('A', result);
  write_flags(result == 0, 0, 0, 0);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}
