#include <stdint.h>

#include "../utils.h"

void nop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void daa(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  // TODO Implement DAA
  UNUSED(instruction);
  UNUSED(cpu_cycles);
  UNUSED(number_of_bytes);
}

void stop(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  // implement STOP
  UNUSED(instruction);
  UNUSED(cpu_cycles);
  UNUSED(number_of_bytes);
}
