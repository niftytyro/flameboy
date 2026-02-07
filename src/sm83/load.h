#ifndef CPU_LOAD_INS
#define CPU_LOAD_INS

#include <stdint.h>

void ld_r8_r8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ld_r8_n8(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ld_r16_n16(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

void ld_HLa_r8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes);

void ld_HLa_n8(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes);

void ld_r8_HLa(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes);

void ld_r16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes);

void ld_n16a_A(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes);

void ldh_n8a_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

void ldh_Ca_A(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ld_A_r16a(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ld_A_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ldh_A_Ca(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ldh_A_n8a(uint8_t *instruction, uint8_t *cpu_cycles,
              uint8_t *number_of_bytes);

void ld_HLIa_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

void ld_HLDa_A(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

void ld_A_HLIa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

void ld_A_HLDa(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes);

#endif
