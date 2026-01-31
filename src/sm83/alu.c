#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../addressing/addressing.h"

uint8_t *registers = NULL;

const int BASE_REGISTER_INDEX = 2;

void boot_cpu() {
  registers = calloc(2 * 6, sizeof(char));
  registers[10] = 0x1;
  registers[11] = 0x50;
  printf("CPU boot successful\n");
}

int extract_register_index_from_low_bits(int low) { return low & 0x7; }

void execute_8_bit_load_instruction(const uint8_t *instruction,
                                    uint8_t *cpu_cycles,
                                    uint8_t *number_of_bytes) {
  int low = *instruction % 0x10;

  int register_index = extract_register_index_from_low_bits(low),
      target_register_index = (*instruction % 0x40) / 0x8;

  if (target_register_index < 0x6) {
    if (register_index < 0x6) {
      registers[BASE_REGISTER_INDEX + target_register_index] =
          registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 0x6) {
      // TODO fix the [HL] value reference
      registers[BASE_REGISTER_INDEX + target_register_index] = -1;
      *cpu_cycles = 2;
    } else if (register_index == 0x7) {
      registers[BASE_REGISTER_INDEX + target_register_index] = registers[0];
    }
  } else if (target_register_index == 0x6) {
    // TODO implement [HL] value reference && also implement 0x76 HALT
  } else {
    if (register_index < 0x6) {
      registers[0] = registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 0x6) {
      // TODO fix the [HL] value reference
      registers[0] = -1;
      *cpu_cycles = 2;
    } else if (register_index == 0x7) {
      registers[0] = registers[0];
    }
  }
}

void execute_8_bit_arithmetic_instruction(const uint8_t *instruction,
                                          uint8_t *cpu_cycles,
                                          uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  uint8_t *accumulator = registers;
  bool should_carry = false;
  uint8_t carry, operand;
  int16_t result = *accumulator;

  if (low > 0x7) {
    should_carry = true;
  }

  if (should_carry && (registers[1] & 0x16) > 0) {
    carry = 1;
  }

  if (high < 0xc) {
    int register_index = extract_register_index_from_low_bits(low);
    if (register_index < 6) {
      operand = registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 6) {
      // TODO fix the [HL] value reference
      operand = -1;
      *cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = registers[0];
    }
  } else {
    if (low != 0x6 && low != 0xe) {
      return;
    }
    operand = *(instruction + 1);
    *number_of_bytes = 2;
    *cpu_cycles = 2;
  }

  // empty the flags
  registers[1] = 0x0;
  if (high == 0x8 || high == 0xc) {
    result = *accumulator + operand + carry;
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
    if (((*accumulator & 0xf) + (operand & 0xf) + carry) > 0xf) {
      registers[1] += 0x20; // 00100000
    }
    if (result > 0xff) {
      registers[1] += 0x10; // 00010000
    }
  } else if (high == 0x9 || high == 0xd) {
    result = *accumulator - operand - carry;
    registers[1] = 0x40; // 01000000
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
    if (((*accumulator & 0xf) - (operand & 0xf) - carry) < 0x0) {
      registers[1] += 0x20; // 00100000
    }
    if (result < 0x0) {
      // fill in the carry flag
      registers[1] += 0x10; // 00010000
    }
  }

  *accumulator = result;
}

void execute_8_bit_logical_instruction(const uint8_t *instruction,
                                       uint8_t *cpu_cycles,
                                       uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  uint8_t *accumulator = registers;
  int operand;
  int16_t result = *accumulator;

  if (high < 0xe) {
    int register_index = extract_register_index_from_low_bits(low);
    if (register_index < 6) {
      operand = registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 6) {
      // TODO fix the [HL] value reference
      operand =
          registers[register_index] * 0x100 + registers[register_index + 1];
      *cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = registers[0];
    }
  } else {
    if (low != 0x6 && low != 0xe) {
      return;
    }
    operand = *(instruction + 1);
    *number_of_bytes = 2;
    *cpu_cycles = 2;
  }

  registers[1] += 0x0;
  if (high == 0xA || high == 0xe) {
    if (low < 0x8) {
      result = *accumulator & operand;
      registers[1] += 0x20; // 00100000
    } else {
      result = *accumulator ^ operand;
    }
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
  } else if ((high == 0xB || high == 0xe) && low < 0x8) {
    result = *accumulator | operand;
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
  }

  *accumulator = result;

  if ((high == 0xB || high == 0xe) && low >= 0x8) {
    result = *accumulator - operand;
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
    registers[1] += 0x40;
    if ((*accumulator & 0xf) - (operand & 0xf) < 0xf) {
      registers[1] += 0x20;
    }
    if (result < 0) {
      registers[1] += 0x10;
    }
  }
}

int execute() {
  printf("---------------------------------------------------------------------"
         "---------CPU "
         "Cycle-------------------------------------------------------"
         "-----------------------\n");
  uint16_t PC = registers[10] * 0x100 + registers[11];
  const uint8_t *instruction = read_address(PC);

  int high = *instruction & 0xf0;
  int low = *instruction % 0x0f;
  // CPU_CYCLES, NUMBER_OF_BYTES
  uint8_t instruction_metadata[2] = {1, 1};

  printf("[ALU Context]\n");
  printf("PC: %02x\n", PC);
  printf("Instruction: 0x%02x\n", *instruction);
  printf("0000000000000\n");

  switch (high) {
  case 0x0:
    break;
  case 0x3:
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7:
    execute_8_bit_load_instruction(instruction, instruction_metadata,
                                   instruction_metadata + 1);
    break;
  case 0x8:
  case 0x9:

    execute_8_bit_arithmetic_instruction(instruction, instruction_metadata,
                                         instruction_metadata + 1);
    break;
  case 0xa:
  case 0xb:
    execute_8_bit_logical_instruction(instruction, instruction_metadata,
                                      instruction_metadata + 1);
    break;
  case 0xc:
    if ((low == 0x6) || (low == 0xe)) {
      execute_8_bit_arithmetic_instruction(instruction, instruction_metadata,
                                           instruction_metadata + 1);
      break;
    }
  // fall through
  case 0xd:
    if ((low == 0x6) || (low == 0xe)) {
      execute_8_bit_arithmetic_instruction(instruction, instruction_metadata,
                                           instruction_metadata + 1);
      break;
    }
  // fall through
  case 0xe:
    if ((low == 0x6) || (low == 0xe)) {
      execute_8_bit_logical_instruction(instruction, instruction_metadata,
                                        instruction_metadata + 1);
      break;
    }
  // fall through
  case 0xf:
    if ((low == 0x6) || (low == 0xe)) {
      execute_8_bit_logical_instruction(instruction, instruction_metadata,
                                        instruction_metadata + 1);
      break;
    }

  default:
    break;
  }

  uint8_t cpu_cycles = instruction_metadata[0];
  uint8_t number_of_bytes = instruction_metadata[1];

  PC += number_of_bytes;
  registers[10] = PC / 0x100;
  registers[11] = PC % 0x100;

  return cpu_cycles;
}
