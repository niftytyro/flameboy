#ifndef CPU_STACK
#define CPU_STACK

#include <stdint.h>

void push_to_stack(uint16_t value);

void add_SP_e8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void dec_SP(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void inc_SP(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_HL_SP(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void ld_SP_n16(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ld_HL_SPe8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ld_SP_HL(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ld_n16a_SP(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void pop_AF(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void pop_r16(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void push_AF(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void push_r16(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
