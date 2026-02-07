#ifndef CPU_JUMPS_INS
#define CPU_JUMPS_INS

#include <stdint.h>

void call_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void call_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void call_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void call_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void call_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void jp_HL(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jp_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jp_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jp_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jp_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jp_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void jr_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jr_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jr_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jr_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void jr_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void ret(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ret_NZ(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ret_Z(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ret_NC(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);
void ret_C(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void reti(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

void rst(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes);

#endif
