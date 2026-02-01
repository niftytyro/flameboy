#ifndef CPU_UTILS
#define CPU_UTILS

#include <stdint.h>

bool is_4bit_carry(uint8_t old_value, uint8_t new_value, bool subtraction);
bool is_8bit_carry(uint8_t old_value, uint8_t new_value, bool subtraction);
bool is_12bit_carry(uint16_t old_value, uint16_t new_value, bool subtraction);
bool is_16bit_carry(uint16_t old_value, uint16_t new_value, bool subtraction);

int extract_half_register_index(int nibble);
int extract_register_index(int nibble);

#endif
