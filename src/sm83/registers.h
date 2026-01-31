#ifndef ADDRESSING_UTILS
#define ADDRESSING_UTILS

#include <stdint.h>

void increment_HL();
void decrement_HL();

uint8_t read_half_register(int i);
uint8_t read_half_register_by_name(char name);

uint16_t read_register(int i);
uint16_t read_register_by_name(char *name);

void write_half_register(int index, uint8_t byte);
void write_half_register_by_name(char name, uint8_t byte);

void write_register(int index, uint8_t byte1, uint8_t byte2);
void write_register_by_name(char *name, uint8_t byte1, uint8_t byte2);

#endif
