#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *registers = NULL;

void boot_cpu() { registers = malloc(sizeof(char) * 2 * 6); }

int execute_8_bit_arithmetic_operation(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;

  char *accumulator = registers;
  bool should_carry = false;
  int carry, operand, base_register_index = 2, register_index = low & 0x7;
  uint8_t result = *accumulator;

  int cpu_cycles = 1;

  if (low > 0x7) {
    should_carry = true;
  }

  if (should_carry && (registers[1] & 0x16) > 0) {
    carry = 1;
  }

  if (register_index < 6) {
    operand = registers[base_register_index + register_index];
  } else if (register_index == 6) {
    // TODO fix the [HL] value reference
    operand = registers[register_index] * 0x100 + registers[register_index + 1];
    cpu_cycles = 2;
  } else if (register_index == 7) {
    operand = registers[0];
  }

  // empty the flags
  registers[1] = 0x0;
  if (high == 0x8) {
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
  } else if (high == 0x9) {
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

int execute_8_bit_logical_operation(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;

  char *accumulator = registers;
  int operand, base_register_index = 2, register_index = low & 0x7;
  uint8_t result = *accumulator;

  int cpu_cycles = 1;

  if (register_index < 6) {
    operand = registers[base_register_index + register_index];
  } else if (register_index == 6) {
    // TODO fix the [HL] value reference
    operand = registers[register_index] * 0x100 + registers[register_index + 1];
    cpu_cycles = 2;
  } else if (register_index == 7) {
    operand = registers[0];
  }

  registers[1] += 0x0;
  if (high == 0xA) {
    if (low < 0x8) {
      result = *accumulator & operand;
      registers[1] += 0x20; // 00100000
    } else {
      result = *accumulator ^ operand;
    }
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
  } else if (high == 0xB && low < 0x8) {
    result = *accumulator | operand;
    if (result == 0) {
      registers[1] += 0x80; // 10000000
    }
  }

  *accumulator = result;

  if (high == 0xB && low >= 0x8) {
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

int calculate(int instruction) {
  int high = instruction / 0x10;
  int cpu_cycles = 0;

  printf("Op code: 0x%x\n", high);

  printf("Value of register AF: %d\n", registers[0] * 0x100 + registers[1]);

  switch (high) {
  case 0x8:
  case 0x9:
    cpu_cycles = execute_8_bit_arithmetic_operation(instruction);
    break;
  case 0xA:
  case 0xB:
    cpu_cycles = execute_8_bit_logical_operation(instruction);
    break;

  default:
    break;
  }

  return cpu_cycles;
}
