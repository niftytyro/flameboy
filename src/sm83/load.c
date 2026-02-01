#include <stdbool.h>
#include <stdint.h>

#include "../addressing/addressing.h"
#include "registers.h"
#include "utils.h"

void ld_r8_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;

  int register_index = extract_half_register_index(low),
      target_register_index = BASE_REGISTER_INDEX + (*instruction % 0x40) / 0x8;

  if (target_register_index != 0x6 && register_index != 0x6) {
    write_half_register(target_register_index,
                        read_half_register(register_index));
  }

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void ld_r8_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;

  int register_index = extract_half_register_index(low);
  uint8_t operand = *(instruction + 1);

  if (register_index != 0x6) {
    write_half_register(register_index, operand);
  }

  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void ld_r16_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;

  int register_index = extract_half_register_index(low);
  uint8_t byte1 = *(instruction + 1), byte2 = *(instruction + 2);

  if (register_index != 0x6) {
    write_register(register_index, byte1, byte2);
  }

  *cpu_cycles = 3;
  *number_of_bytes = 3;
}

void ld_HLa_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;
  int register_index = extract_half_register_index(low);

  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register(register_index);

  write_address(address, byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_HLa_n8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = *(instruction + 1);

  write_address(address, byte);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ld_r8_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;
  int register_index = extract_half_register_index(low);

  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register(register_index, *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_r16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;
  int register_index = extract_register_index(low);

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

void ldh_n16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t address = 0xff00 + *(instruction + 1);
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void ldh_Ca_A(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint16_t address = 0xff00 + read_half_register_by_name('C');
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_r16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  int low = *instruction & 0xf;
  int register_index = extract_register_index(low);

  uint16_t address = read_register(register_index);
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ld_A_n16(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint16_t address = *(instruction + 2) * 0x100 + *(instruction + 1);
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 4;
  *number_of_bytes = 3;
}

void ldh_A_Ca(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes) {
  uint16_t address = 0xff00 + read_half_register_by_name('C');
  uint8_t *byte = read_address(address);

  write_half_register_by_name('A', *byte);

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ldh_HLIa_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  increment_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ldh_HLDa_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t byte = read_half_register_by_name('A');

  write_address(address, byte);

  decrement_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ldh_A_HLIa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register('A', *byte);

  increment_register_by_name("HL");

  *cpu_cycles = 2;
  *number_of_bytes = 1;
}

void ldh_A_HLDa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");
  uint8_t *byte = read_address(address);

  write_half_register('A', *byte);

  decrement_register_by_name("HL");

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
