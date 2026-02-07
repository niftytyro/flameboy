#ifndef CPU_MISC_INS
#define CPU_MISC_INS

#include <stdint.h>

void nop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void daa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void stop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
