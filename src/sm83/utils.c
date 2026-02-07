#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "registers.h"

bool is_4bit_carry(uint8_t old_value, uint8_t new_value, bool subtraction) {
  // TODO confirm if this will always work
  if (subtraction) {
    return (new_value & 0xf) > (old_value & 0xf);
  }

  return (new_value & 0xf) < (old_value & 0xf);
}

bool is_8bit_carry(uint8_t old_value, uint8_t new_value, bool subtraction) {
  if (subtraction) {
    return new_value > old_value;
  }

  return new_value < old_value;
}

bool is_12bit_carry(uint16_t old_value, uint16_t new_value, bool subtraction) {
  if (subtraction) {
    return (new_value & 0x0f00) > (old_value & 0x0f00);
  }

  return (new_value & 0x0f00) < (old_value & 0x0f00);
}

bool is_16bit_carry(uint16_t old_value, uint16_t new_value, bool subtraction) {
  if (subtraction) {
    return new_value > old_value;
  }

  return new_value < old_value;
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

  int i = high % base;
  i += BASE_REGISTER_INDEX;

  printf("Register Index: %d\n", i);

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

  if (low == 0xe) {
    i++;
  }

  i %= 0x9;

  printf("Register Index (r8): %d\n", i);

  return i;
}
