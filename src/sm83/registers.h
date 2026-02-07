#ifndef CPU_REGISTERS
#define CPU_REGISTERS

#include <stdint.h>
#include <stdbool.h>

extern const int BASE_REGISTER_INDEX;


void initialize_registers();

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
uint8_t write_flags(bool z, bool n, bool h, bool c);

#endif

