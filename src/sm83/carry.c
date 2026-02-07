#include <stdbool.h>
#include <stdint.h>

#include "../utils.h"
#include "registers.h"

void ccf(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  bool carry = read_flag('C');

  write_flags(read_flag('Z'), 0, 0, !carry);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void scf(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  UNUSED(instruction);
  write_flags(read_flag('Z'), 0, 0, 1);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}
