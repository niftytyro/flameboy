#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../addressing/addressing.h"
#include "registers.h"

const int BASE_REGISTER_INDEX = 2;

void boot_cpu() { printf("CPU boot successful\n"); }

int extract_register_index_from_low_bits(int low) { return low & 0x7; }

void execute_load_instruction(uint8_t *instruction, uint8_t *cpu_cycles,
                              uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  if (high < 0x4) {
    int register_index = BASE_REGISTER_INDEX + high * 2;
    if (low == 0x1) {
      if (high == 0x0) {
        write_register(register_index, *(instruction + 1), *(instruction + 2));
      }

      *cpu_cycles = 3;
      *number_of_bytes = 3;
    } else if (low == 0x2) {
      uint16_t address = read_register(register_index);
      write_address(address, read_half_register_by_name('A'));

      if (high >= 0x2) {
        increment_HL();
      }

      *cpu_cycles = 2;
    }
  } else if (high < 0x8) {
    int register_index = extract_register_index_from_low_bits(low),
        target_register_index = (*instruction % 0x40) / 0x8;

    if (target_register_index < 0x6) {
      if (register_index < 0x6) {
        write_half_register(
            BASE_REGISTER_INDEX + target_register_index,
            read_half_register(BASE_REGISTER_INDEX + register_index));
      } else if (register_index == 0x6) {
        // TODO fix the [HL] value reference
        write_half_register(BASE_REGISTER_INDEX + target_register_index, -1);
        *cpu_cycles = 2;
      } else if (register_index == 0x7) {
        write_half_register(BASE_REGISTER_INDEX + target_register_index,
                            read_half_register_by_name('A'));
      }
    } else if (target_register_index == 0x6) {
      // TODO implement [HL] value reference && also implement 0x76 HALT
    } else {
      if (register_index < 0x6) {
        write_half_register_by_name(
            'A', read_half_register(BASE_REGISTER_INDEX + register_index));
      } else if (register_index == 0x6) {
        // TODO fix the [HL] value reference
        write_half_register_by_name('A', -1);
        *cpu_cycles = 2;
      } else if (register_index == 0x7) {
        uint16_t address = read_register_by_name("HL");
        uint8_t *byte = read_address(address);
        write_half_register_by_name('A', *byte);
      }
    }
    return;
  }
}

void execute_8_bit_arithmetic_instruction(uint8_t *instruction,
                                          uint8_t *cpu_cycles,
                                          uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  uint8_t accumulator = read_half_register_by_name('A'),
          flags = read_half_register_by_name('F');
  bool should_carry = false;
  uint8_t carry, operand;
  int16_t result = accumulator;

  if (low > 0x7) {
    should_carry = true;
  }

  if (should_carry && (flags & 0x10) > 0) {
    carry = 1;
  }

  if (high < 0xc) {
    int register_index = extract_register_index_from_low_bits(low);
    if (register_index < 6) {
      operand = read_half_register(BASE_REGISTER_INDEX + register_index);
    } else if (register_index == 6) {
      // TODO fix the [HL] value reference
      operand = -1;
      *cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = read_half_register('A');
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
  flags = 0x0;
  if (high == 0x8 || high == 0xc) {
    result = accumulator + operand + carry;
    if (result == 0) {
      flags += 0x80; // 10000000
    }
    if (((accumulator & 0xf) + (operand & 0xf) + carry) > 0xf) {
      flags += 0x20; // 00100000
    }
    if (result > 0xff) {
      flags += 0x10; // 00010000
    }
  } else if (high == 0x9 || high == 0xd) {
    result = accumulator - operand - carry;
    flags = 0x40; // 01000000
    if (result == 0) {
      flags += 0x80; // 10000000
    }
    if (((accumulator & 0xf) - (operand & 0xf) - carry) < 0x0) {
      flags += 0x20; // 00100000
    }
    if (result < 0x0) {
      // fill in the carry flag
      flags += 0x10; // 00010000
    }
  }

  write_half_register_by_name('A', result);
  write_half_register_by_name('F', flags);
}

void execute_8_bit_logical_instruction(uint8_t *instruction,
                                       uint8_t *cpu_cycles,
                                       uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  uint8_t accumulator = read_half_register_by_name('A'),
          flags = read_half_register_by_name('F');
  int operand;
  int16_t result = accumulator;

  if (high < 0xe) {
    int register_index = extract_register_index_from_low_bits(low);
    if (register_index < 6) {
      operand = read_half_register(BASE_REGISTER_INDEX + register_index);
    } else if (register_index == 6) {
      // TODO fix the [HL] value reference
      *cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = read_half_register(0);
    }
  } else {
    if (low != 0x6 && low != 0xe) {
      return;
    }
    operand = *(instruction + 1);
    *number_of_bytes = 2;
    *cpu_cycles = 2;
  }

  flags = 0x0;
  if (high == 0xA || high == 0xe) {
    if (low < 0x8) {
      result = accumulator & operand;
      flags += 0x20; // 00100000
    } else {
      result = accumulator ^ operand;
    }
    if (result == 0) {
      flags += 0x80; // 10000000
    }
  } else if ((high == 0xB || high == 0xe) && low < 0x8) {
    result = accumulator | operand;
    if (result == 0) {
      flags += 0x80; // 10000000
    }
  }

  write_half_register_by_name('A', result);

  flags = 0x0;
  if ((high == 0xB || high == 0xe) && low >= 0x8) {
    result = accumulator - operand;
    if (result == 0) {
      flags += 0x80; // 10000000
    }
    flags += 0x40;
    if ((accumulator & 0xf) - (operand & 0xf) < 0xf) {
      flags += 0x20;
    }
    if (result < 0) {
      flags += 0x10;
    }
  }

  write_half_register_by_name('F', flags);
}

int execute() {
  printf("---------------------------------------------------------------------"
         "---------CPU "
         "Cycle-------------------------------------------------------"
         "-----------------------\n");
  uint16_t PC = read_register_by_name("PC");
  uint8_t *instruction = read_address(PC);

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
    if (low == 0x0) {
      // NOP
      break;
    }
    if (low < 0x3) {
      execute_load_instruction(instruction, instruction_metadata,
                               instruction_metadata + 1);
      break;
    }
  case 0x3:
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7:
    execute_load_instruction(instruction, instruction_metadata,
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
  write_register_by_name("PC", PC / 0x100, PC % 0x100);

  return cpu_cycles;
}
