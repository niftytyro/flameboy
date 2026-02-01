#ifndef CPU_ARITHMETIC_INS
#define CPU_ARITHMETIC_INS

#include <stdint.h>

void adc_A_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void adc_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void adc_A_n8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_A_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_A_n8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_HL_r16(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_HL_SP(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void add_SP_e8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
