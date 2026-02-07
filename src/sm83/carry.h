#ifndef CPU_CARRY_INS
#define CPU_CARRY_INS


#include <stdint.h>

void ccf(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void scf(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
