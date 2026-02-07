#ifndef CPU_INTERRUPTS_INS
#define CPU_INTERRUPTS_INS

#include <stdint.h>

void di(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void ei(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void halt(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
