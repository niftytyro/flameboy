#ifndef CPU_BIT_MANIPULATION_INS
#define CPU_BIT_MANIPULATION_INS

#include <stdint.h>

void bit_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void bit_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void res_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void res_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void set_u3_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void set_u3_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void rl_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rl_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rla(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rlc_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rlc_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rlca(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void rr_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rr_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rra(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rrc_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rrc_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void rrca(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void sla_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void sla_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void sra_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void sra_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void srl_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void srl_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void swap_r8(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void swap_HLa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
