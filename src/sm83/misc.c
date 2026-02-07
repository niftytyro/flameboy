#include <stdint.h>

void nop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void daa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {}

void stop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
}
