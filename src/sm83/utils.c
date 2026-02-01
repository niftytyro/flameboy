#include <stdbool.h>
#include <stdint.h>

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
