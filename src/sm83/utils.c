#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "registers.h"

bool is_4bit_carry(uint8_t value, uint8_t operand, uint8_t carry,
                   bool subtraction) {
  if ((carry == 0 && operand == 0) || (carry + operand == value)) {
    return 0;
  }
  if (subtraction) {
    uint8_t result = value - (operand + carry);
    return (result & 0xf) >= (value & 0xf);
  }

  uint8_t result = value + operand + carry;
  return (result & 0xf) <= (value & 0xf);
}

bool is_8bit_carry(uint8_t value, uint8_t operand, uint8_t carry,
                   bool subtraction) {
  if ((carry == 0 && operand == 0) || (carry + operand == value)) {
    return 0;
  }
  if (subtraction) {
    uint8_t result = value - (operand + carry);
    return (result >= value);
  }

  uint8_t result = value + operand + carry;
  return (result <= value);
}

bool is_12bit_carry(uint8_t value, uint8_t operand, uint8_t carry,
                    bool subtraction) {
  if ((carry == 0 && operand == 0) || (carry + operand == value)) {
    return 0;
  }

  if (subtraction) {
    uint16_t result = value - (operand + carry);
    return (result & 0x0f00) >= (value & 0x0f00);
  }

  uint16_t result = value + operand + carry;
  return (result & 0x0f00) <= (value & 0x0f00);
}

bool is_16bit_carry(uint8_t value, uint8_t operand, uint8_t carry,
                    bool subtraction) {
  if ((carry == 0 && operand == 0) || (carry + operand == value)) {
    return 0;
  }
  if (subtraction) {
    uint16_t result = value - (operand + carry);
    return (result >= value);
  }

  uint16_t result = value + operand + carry;
  return (result <= value);
}

int extract_half_register_index_for_grouped_ins(int instruction) {
  uint8_t low = instruction & 0xf;
  int i = low % 0x8;
  i += BASE_REGISTER_INDEX;
  i %= 0x9;

  printf("Register Index (half): %d\n", i);

  return i;
}

int extract_register_index(uint8_t instruction, uint8_t base) {
  uint8_t high = instruction / 0x10;

  int i = high;
  if (base != 0) {
    i = high % base;
  }

  i *= 2;
  i += BASE_REGISTER_INDEX;

  return i;
}

int extract_register_index_r8(uint8_t instruction, uint8_t base) {
  uint8_t high = instruction / 0x10;
  uint8_t low = instruction % 0x10;

  int i = high;
  if (base != 0) {
    i = high % base;
  }

  i *= 2;
  i += BASE_REGISTER_INDEX;

  if (low > 0x7) {
    i++;
  }

  i %= 0x9;

  printf("Register Index (r8): %d\n", i);

  return i;
}
