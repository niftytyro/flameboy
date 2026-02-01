#ifndef CPU_LOGIC_INS
#define CPU_LOGIC_INS

#include <stdint.h>

void and_A_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void and_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void and_A_n8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void cpl(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void or_A_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void or_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void or_A_n8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void xor_A_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void xor_A_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void xor_A_n8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
