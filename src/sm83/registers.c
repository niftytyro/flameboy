#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "registers.h"

uint8_t *registers = NULL;

const int BASE_REGISTER_INDEX = 2;

void initialize_registers() {
  registers = calloc(2 * 6, sizeof(char));
  // TODO remove - added for testing
  registers[10] = 0x1;
  registers[11] = 0x50;
}

uint8_t write_flags(bool z, bool n, bool h, bool c) {
  return write_half_register_by_name('F',
                                     c * 0x10 + h * 0x20 + n * 0x40 + z * 0x80);
}

uint8_t read_flag(char name) {
  uint8_t flags = read_half_register_by_name('F');

  if (name == 'z') {
    return flags & 0x80;
  }
  if (name == 'n') {
    return flags & 0x40;
  }
  if (name == 'h') {
    return flags & 0x20;
  }
  if (name == 'c') {
    return flags & 0x10;
  }

  return -1;
}

uint8_t update_flags(bool negative, uint16_t old_value, uint16_t new_value,
                     uint16_t operand) {
  uint8_t z, n, h, c;
  z = new_value == 0;
  n = negative;

  if (negative) {
    c = new_value < 0x00;
    h = (old_value >> 3 & 0x1) == 0 && (operand >> 3 & 0x1) == 1;
  } else {
    c = new_value > 0xff;
    h = ((new_value >> 3 & 0x1) == 1) && ((new_value >> 3 & 0x1) == 0);
  }

  return write_flags(z, n, h, c);
}

int get_register_index(char name) {
  if (name == 'A') {
    return 0;
  }
  if (name == 'F') {
    return 1;
  }
  if (name == 'B') {
    return 2;
  }
  if (name == 'C') {
    return 3;
  }
  if (name == 'D') {
    return 4;
  }
  if (name == 'E') {
    return 5;
  }
  if (name == 'H') {
    return 6;
  }
  if (name == 'L') {
    return 7;
  }
  if (name == 'S') {
    return 8;
  }
  if (name == 'P') {
    return 10;
  }

  return 0;
}

uint8_t read_half_register(int index) { return registers[index]; }

uint8_t read_half_register_by_name(char name) {
  return registers[get_register_index(name)];
}

uint16_t read_register(int i) {
  return registers[i] * 0x100 + registers[i + 1];
}

uint16_t read_register_by_name(char *name) {
  int i = get_register_index(name[0]);

  return registers[i] * 0x100 + registers[i + 1];
}

uint8_t write_half_register_by_name(char name, uint8_t byte) {
  registers[get_register_index(name)] = byte;
  return byte;
}

uint8_t write_half_register(int index, uint8_t byte) {
  registers[index] = byte;
  return byte;
}

uint16_t write_register_by_name(char *name, uint8_t byte1, uint8_t byte2) {
  int i = get_register_index(name[0]);

  registers[i] = byte1;
  registers[i + 1] = byte2;

  return registers[i] * 0x100 + registers[i + 1];
}

uint16_t write_register(int index, uint8_t byte1, uint8_t byte2) {
  int i = get_register_index(index);

  registers[i] = byte1;
  registers[i + 1] = byte2;

  return registers[i] * 0x100 + registers[i + 1];
}

uint8_t increment_half_register(int i) {
  uint8_t value = read_half_register(i);

  uint8_t new_value = write_half_register(i, value + 1);
  uint8_t half_carry =
      ((value >> 3 & 0x1) == 1) && ((new_value >> 3 & 0x1) == 0);

  return write_flags(value == 0, 0, half_carry, read_flag('c'));
}

uint8_t decrement_half_register(int i) {
  uint8_t value = read_half_register(i);

  value--;

  return write_half_register(i, value);
}

uint16_t increment_register(int i) {
  uint16_t value = read_register(i);

  value++;

  return write_register(i, value / 0x100, value % 0x100);
}

uint16_t decrement_register(int i) {
  uint16_t value = read_register(i);

  value--;

  return write_register(i, value / 0x100, value % 0x100);
}

uint8_t increment_half_register_by_name(char name) {
  uint8_t value = read_half_register_by_name(name);

  value++;

  return write_half_register_by_name(name, value);
}

uint8_t decrement_half_register_by_name(char name) {
  uint8_t value = read_half_register_by_name(name);

  value--;

  return write_half_register_by_name(name, value);
}

uint16_t increment_register_by_name(char *name) {
  uint16_t value = read_register_by_name(name);

  value++;

  return write_register_by_name(name, value / 0x100, value % 0x100);
}

uint16_t decrement_register_by_name(char *name) {
  uint16_t value = read_register_by_name(name);

  value--;

  return write_register_by_name(name, value / 0x100, value % 0x100);
}
