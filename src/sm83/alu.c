#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *registers = NULL;

const int BASE_REGISTER_INDEX = 2;

void boot_cpu() { registers = malloc(sizeof(char) * 2 * 6); }

int extract_register_index_from_low_bits(int low) { return low & 0x7; }

int execute_8_bit_load_instruction(int instruction) {
  int low = instruction % 0x10;

  int register_index = extract_register_index_from_low_bits(low),
      target_register_index = (instruction % 0x40) / 0x8, cpu_cycles = 1;

  if (target_register_index < 0x6) {
    if (register_index < 0x6) {
      registers[BASE_REGISTER_INDEX + target_register_index] =
          registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 0x6) {
      // TODO fix the [HL] value reference
      registers[BASE_REGISTER_INDEX + target_register_index] = -1;
      cpu_cycles = 2;
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
      cpu_cycles = 2;
    } else if (register_index == 0x7) {
      registers[0] = registers[0];
    }
  }

  return cpu_cycles;
}

int execute_8_bit_arithmetic_instruction(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;

  char *accumulator = registers;
  bool should_carry = false;
  int carry, operand;
  uint8_t result = *accumulator;

  int cpu_cycles = 1;

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
      cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = registers[0];
    }
  } else {
    operand = low;
    cpu_cycles = 2;
  }

  // empty the flags
  registers[1] = 0x0;
  if (high == 0x8 || high == 0xc) {
    result = *accumulator + operand + carry;
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
    if ((*accumulator & 0xf + operand & 0xf + carry) > 0xf) {
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
    if ((*accumulator & 0xf - operand & 0xf - carry) < 0x0) {
      registers[1] += 0x20; // 00100000
    }
    if (result < 0x0) {
      // fill in the carry flag
      registers[1] += 0x10; // 00010000
    }
  }

  *accumulator = result;

  return cpu_cycles;
}

int execute_8_bit_logical_instruction(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;

  char *accumulator = registers;
  int operand;
  uint8_t result = *accumulator;

  int cpu_cycles = 1;

  if (high < 0xe) {
    int register_index = extract_register_index_from_low_bits(low);
    if (register_index < 6) {
      operand = registers[BASE_REGISTER_INDEX + register_index];
    } else if (register_index == 6) {
      // TODO fix the [HL] value reference
      operand =
          registers[register_index] * 0x100 + registers[register_index + 1];
      cpu_cycles = 2;
    } else if (register_index == 7) {
      operand = registers[0];
    }
  } else {
    operand = low;
    cpu_cycles = 2;
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
    if (*accumulator & 0xf - operand & 0xf < 0xf) {
      registers[1] += 0x20;
    }
    if (result < 0) {
      registers[1] += 0x10;
    }
  }

  return cpu_cycles;
}

int execute_instruction(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;
  int cpu_cycles = 1;

  printf("Op code: 0x%x\n", high);

  printf("Value of register AF: %d\n", registers[0] * 0x100 + registers[1]);

  switch (high) {
  case 0x00:
    break;
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7:
    cpu_cycles = execute_8_bit_load_instruction(instruction);
    break;
  case 0x8:
  case 0x9:
    cpu_cycles = execute_8_bit_arithmetic_instruction(instruction);
    break;
  case 0xa:
  case 0xb:
    cpu_cycles = execute_8_bit_logical_instruction(instruction);
    break;
  case 0xc:
    if (low == 0x6 || low == 0xe) {
      cpu_cycles = execute_8_bit_arithmetic_instruction(instruction);
      break;
    }
  case 0xd:
    if (low == 0x6 || low == 0xe) {
      cpu_cycles = execute_8_bit_arithmetic_instruction(instruction);
      break;
    }
  case 0xe:
    if (low == 0x6 || low == 0xe) {
      cpu_cycles = execute_8_bit_logical_instruction(instruction);
      break;
    }
  case 0xf:
    if (low == 0x6 || low == 0xe) {
      cpu_cycles = execute_8_bit_logical_instruction(instruction);
      break;
    }

  default:
    break;
  }

  return cpu_cycles;
}
