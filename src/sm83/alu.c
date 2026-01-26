#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// r8
// r16
// n8
// n16

char *registers = NULL;

void boot_cpu() { registers = malloc(sizeof(char) * 2 * 6); }

int execute_8_bit_arithmetic_operation(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;

  // internal variables
  bool should_carry = false;
  int carry, operand, base_register_index = 2, register_index = low & 0x7,
                      total = 0;
  char *accumulator = registers;

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
    operand = registers[register_index] * 0x100 + registers[register_index + 1];
    cpu_cycles = 2;
  } else if (register_index == 7) {
    operand = registers[0];
  }

  // empty the carry flag
  registers[1] &= 0xef; // 11101111

  if (high == 0x8) {
    total = *accumulator + operand + carry;
    if (total > 0xff) {
      // fill in the carry flag
      registers[1] += 0x16; // 00010000
    }
  } else if (high == 0x9) {
    total = *accumulator - operand - carry;
    if (total < 0x0) {
      // fill in the carry flag
      registers[1] += 0x16; // 00010000
    }
  }

  *accumulator = total;

  return cpu_cycles;
}

int calculate(int instruction) {
  int high = instruction / 0x10;
  int low = instruction % 0x10;
  int cpu_cycles = 0;

  printf("Op code: 0x%x%x\n", high, low);

  printf("Value of register AF: %d\n", registers[0] * 0x100 + registers[1]);

  switch (high) {
  case 0x8:
  case 0x9:
    cpu_cycles = execute_8_bit_arithmetic_operation(instruction);
    break;

  default:
    break;
  }

  return cpu_cycles;
}
