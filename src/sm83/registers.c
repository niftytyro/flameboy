#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *registers = NULL;

void initialize_registers() {
  registers = calloc(2 * 6, sizeof(char));
  // TODO remove - added for testing
  registers[10] = 0x1;
  registers[11] = 0x50;
}

void increment_HL() {
  uint16_t HL = registers[6] * 0x100 + registers[7];

  HL++;

  registers[6] = HL & 0xff00;
  registers[7] = HL & 0x00ff;
}

void decrement_HL() {
  uint16_t HL = registers[6] * 0x100 + registers[7];

  HL--;

  registers[6] = HL & 0xff00;
  registers[7] = HL & 0x00ff;
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

void write_half_register_by_name(char name, uint8_t byte) {
  registers[get_register_index(name)] = byte;
}

void write_half_register(int index, uint8_t byte) { registers[index] = byte; }

void write_register_by_name(char *name, uint8_t byte1, uint8_t byte2) {

  int i = get_register_index(name[0]);

  registers[i] = byte1;
  registers[i + 1] = byte2;
}

void write_register(int index, uint8_t byte1, uint8_t byte2) {

  int i = get_register_index(index);

  registers[i] = byte1;
  registers[i + 1] = byte2;
}
