#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../addressing/addressing.h"
#include "../utils.h"
#include "registers.h"
#include "utils.h"

void ld_r8_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  int register_index =
          extract_half_register_index_for_grouped_ins(*instruction),
      target_register_index =
          (BASE_REGISTER_INDEX + (*instruction % 0x40) / 0x8) % 0x9;

  write_half_register(target_register_index,
                      read_half_register(register_index));

  printf("ld %d %d\n", target_register_index, register_index);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void ld_r8_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {

  int register_index = extract_register_index_r8(*instruction, 0);
  uint8_t operand = *(instruction + 1);

  write_half_register(register_index, operand);

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void ld_r16_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  int register_index = extract_register_index_r8(*instruction, 0);
  uint8_t byte1 = *(instruction + 2), byte2 = *(instruction + 1);

  write_register(register_index, byte1, byte2);

  printf("ld %d 0x%02x%02x\n", register_index, byte1, byte2);

  *cpu_cycles = 3;
  *number_of_bytes = 3;
}

void ld_HLa_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int register_index =
      extract_half_register_index_for_grouped_ins(*instruction);

  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register(register_index);

  write_address(address, byte);

  printf("ld 0x%04x 0x%02x\n", address, byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_HLa_n8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = *(instruction + 1);

  write_address(address, byte);

  printf("ld 0x%04x 0x%02x\n", address, byte);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ld_r8_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int register_index =
      (BASE_REGISTER_INDEX + (*instruction % 0x40) / 0x8) % 0x9;

  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register(register_index, *byte);

  printf("ld %d 0x%02x\n", register_index, *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_r16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int register_index = extract_register_index(*instruction, 0);

  uint16_t address = read_register(register_index);
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_n16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = *(instruction + 2) * 0x100 + *(instruction + 1);
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 4;
  *number_of_bytes = 3;
}

void ldh_n8a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = 0xff00 + *(instruction + 1);
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ldh_Ca_A(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = 0xff00 + read_half_register_by_name('C');
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_r16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int register_index = extract_register_index(*instruction, 0);

  uint16_t address = read_register(register_index);
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = *(instruction + 2) * 0x100 + *(instruction + 1);
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 4;
  *number_of_bytes = 3;
}

void ldh_A_n8a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = 0xff00 + *(instruction + 1);
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ldh_A_Ca(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = 0xff00 + read_half_register_by_name('C');
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_HLIa_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  increment_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_HLDa_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  decrement_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_HLIa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  increment_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_HLDa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  UNUSED(instruction);
  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  decrement_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}
