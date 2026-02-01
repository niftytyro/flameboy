#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../addressing/addressing.h"
#include "registers.h"

const int BASE_REGISTER_INDEX = 2;

void boot_cpu() { printf("CPU boot successful\n"); }

int extract_half_register_index(int low) { return low & 0x7; }

void execute_byte_rotation_instruction(uint8_t *instruction,
                                       uint8_t *cpu_cycles,
                                       uint8_t *number_of_bytes) {
  uint8_t accumulator = read_half_register_by_name('A');
  uint8_t flags = read_half_register_by_name('F');

  uint8_t bit_7 = accumulator / 0x80;

  // rotate A by 1
  accumulator = accumulator << 1;
  accumulator += bit_7;

  // update carry flag based on the result
  flags = flags & (0xef + (bit_7 * 0x10));

  write_register_by_name("AF", accumulator, flags);
}

void execute_inc_dec_instruction(uint8_t *instruction, uint8_t *cpu_cycles,
                                 uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  int register_index = BASE_REGISTER_INDEX + high * 2;

  switch (low) {
  case 0x3:
    increment_register(register_index);
    break;
  case 0x4: {
    increment_half_register(register_index);
    break;
  }
  case 0x5:
    decrement_half_register(register_index);
    break;
  case 0xb:
    decrement_register(register_index);
    break;
  case 0xc:
    increment_half_register(register_index + 1);
    break;
  case 0xd:
    decrement_half_register(register_index + 1);
    break;
  }
}

void execute_8_bit_load_value_instruction(uint8_t *instruction,
                                          uint8_t *cpu_cycles,
                                          uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;

  int register_index = BASE_REGISTER_INDEX + high * 2;
  uint8_t value = *(instruction + 1);

  write_half_register(register_index, value);
  *cpu_cycles = 2;
  *number_of_bytes = 2;
}

void execute_8_bit_load_from_register_instruction(uint8_t *instruction,
                                                  uint8_t *cpu_cycles,
                                                  uint8_t *number_of_bytes) {
  int low = *instruction % 0x10;

  int register_index = extract_half_register_index(low),
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

void execute_16_bit_load_instruction(uint8_t *instruction, uint8_t *cpu_cycles,
                                     uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  int register_index = BASE_REGISTER_INDEX + high * 2;
  if (low == 0x1) {
    write_register(register_index, *(instruction + 1), *(instruction + 2));

    *cpu_cycles = 3;
    *number_of_bytes = 3;
  } else if (low == 0x2) {
    uint16_t address = read_register(register_index);
    write_address(address, read_half_register_by_name('A'));

    if (high == 0x2) {
      increment_register_by_name("HL");
    } else if (high == 0x3) {
      decrement_register_by_name("HL");
    }

    *cpu_cycles = 2;
  }
}

void execute_load_instruction(uint8_t *instruction, uint8_t *cpu_cycles,
                              uint8_t *number_of_bytes) {
  int high = *instruction / 0x10;
  int low = *instruction % 0x10;

  if (high < 0x4) {
    if (low < 0x3) {
      execute_16_bit_load_instruction(instruction, cpu_cycles, number_of_bytes);
    }
    if (low == 0x6) {
      execute_8_bit_load_value_instruction(instruction, cpu_cycles,
                                           number_of_bytes);
    }
  } else if (high < 0x8) {
    execute_8_bit_load_from_register_instruction(instruction, cpu_cycles,
                                                 number_of_bytes);
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

  if (high < 0xa) {
    int register_index = extract_half_register_index(low);
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

  if (high == 0x8 || high == 0xc) {
    result = accumulator + operand + carry;
    update_flags(0, accumulator, result, operand + carry);
  } else if (high == 0x9 || high == 0xd) {
    result = accumulator - operand - carry;
    update_flags(1, accumulator, result, operand + carry);
  }

  write_half_register_by_name('A', result);
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
    int register_index = extract_half_register_index(low);
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

  flags = 0;
  if (high == 0xa || high == 0xe) {
    if (low < 0x8) {
      result = accumulator & operand;
      write_flags(result == 0, 0, 1, 0);
    } else {
      result = accumulator ^ operand;
      write_flags(result == 0, 0, 0, 0);
    }
  } else if ((high == 0xb || high == 0xf) && low < 0x8) {
    result = accumulator | operand;
    write_flags(result == 0, 0, 0, 0);
  }

  write_half_register_by_name('A', result);

  if ((high == 0xb || high == 0xf) && low >= 0x8) {
    result = accumulator - operand;
    update_flags(1, accumulator, result, operand);
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
    if (low < 0x3 || low == 0x6) {
      execute_load_instruction(instruction, instruction_metadata,
                               instruction_metadata + 1);
      break;
    }
    if (low < 0x6) {
      execute_inc_dec_instruction(instruction, instruction_metadata,
                                  instruction_metadata + 1);
      break;
    }
    if (low == 0x7) {
      execute_byte_rotation_instruction(instruction, instruction_metadata,
                                        instruction_metadata + 1);
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
