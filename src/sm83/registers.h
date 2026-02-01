#ifndef ADDRESSING_UTILS
#define ADDRESSING_UTILS

#include <stdint.h>

extern const int BASE_REGISTER_INDEX;

uint8_t read_half_register(int i);
uint8_t read_half_register_by_name(char name);

uint16_t read_register(int i);
uint16_t read_register_by_name(char *name);

uint8_t write_half_register(int index, uint8_t byte);
uint8_t write_half_register_by_name(char name, uint8_t byte);

uint16_t write_register(int index, uint8_t byte1, uint8_t byte2);
uint16_t write_register_by_name(char *name, uint8_t byte1, uint8_t byte2);

uint16_t increment_register(int i);
uint16_t decrement_register(int i);
uint8_t increment_half_register(int i);
uint8_t decrement_half_register(int i);

uint16_t increment_register_by_name(char *name);
uint16_t decrement_register_by_name(char *name);
uint8_t increment_half_register_by_name(char name);
uint8_t decrement_half_register_by_name(char name);

uint8_t read_flag(char name);
uint8_t write_flags(uint8_t z, uint8_t n, uint8_t h, uint8_t c);
uint8_t update_flags(bool negative, uint16_t old_value, uint16_t new_value, uint16_t operand);

#endif

