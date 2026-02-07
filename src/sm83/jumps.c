#include <stdbool.h>
#include <stdint.h>

#include "registers.h"
#include "stack.h"

void call_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("PC");
  uint16_t target_address = *(instruction + 2) * 0x100 + *(instruction + 1);

  push_to_stack(address);

  write_register_by_name("PC", target_address / 0x100, target_address % 0x100);

  *cpu_cycles = 6;
  *number_of_bytes = 3;
}

void call_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                  uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (!is_zero) {
    call_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                 uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (is_zero) {
    call_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                  uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (!is_carry) {
    call_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void call_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                 uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (is_carry) {
    call_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void jp_HL(uint8_t *instruction, uint8_t *cpu_cycles,
           uint8_t *number_of_bytes) {
  uint16_t address = read_register_by_name("HL");

  write_register_by_name("PC", address / 0x100, address % 0x100);

  *cpu_cycles = 1;
  *number_of_bytes = 1;
}

void jp_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  uint16_t address = *(instruction + 2) * 0x100 + *(instruction + 1);

  write_register_by_name("PC", address / 0x100, address % 0x100);

  *cpu_cycles = 4;
  *number_of_bytes = 3;
}

void jp_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (!is_zero) {
    jp_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void jp_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (is_zero) {
    jp_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void jp_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (!is_carry) {
    jp_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void jp_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (is_carry) {
    jp_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 3;
  }

  *number_of_bytes = 3;
}

void jr_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
             uint8_t *number_of_bytes) {
  char offset = *(instruction + 1);
  uint16_t address = read_register_by_name("PC");

  address += offset;

  write_register_by_name("PC", address / 0x100, address % 0x100);

  *cpu_cycles = 3;
  *number_of_bytes = 2;
}

void jr_NZ_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (!is_zero) {
    jr_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 2;
}

void jr_Z_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (is_zero) {
    jr_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 2;
}

void jr_NC_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
                uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (!is_carry) {
    jr_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 2;
}

void jr_C_n16a(uint8_t *instruction, uint8_t *cpu_cycles,
               uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (is_carry) {
    jr_n16a(instruction, cpu_cycles, number_of_bytes);
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 2;
}

void ret(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  write_register_by_name("PC", read_half_register_by_name('S'),
                         read_half_register_by_name('P'));

  *cpu_cycles = 4;
  *number_of_bytes = 1;
}

void ret_NZ(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (!is_zero) {
    ret(instruction, cpu_cycles, number_of_bytes);
    *cpu_cycles = 5;
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 1;
}

void ret_Z(uint8_t *instruction, uint8_t *cpu_cycles,
           uint8_t *number_of_bytes) {
  bool is_zero = read_flag('Z');
  if (is_zero) {
    ret(instruction, cpu_cycles, number_of_bytes);
    *cpu_cycles = 5;

  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 1;
}

void ret_NC(uint8_t *instruction, uint8_t *cpu_cycles,
            uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (!is_carry) {
    ret(instruction, cpu_cycles, number_of_bytes);
    *cpu_cycles = 5;
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 1;
}

void ret_C(uint8_t *instruction, uint8_t *cpu_cycles,
           uint8_t *number_of_bytes) {
  bool is_carry = read_flag('C');
  if (is_carry) {
    ret(instruction, cpu_cycles, number_of_bytes);
    *cpu_cycles = 5;
  } else {
    *cpu_cycles = 2;
  }

  *number_of_bytes = 1;
}

void reti(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  // TODO implement
}

void rst(uint8_t *instruction, uint8_t *cpu_cycles, uint8_t *number_of_bytes) {
  uint8_t high = *instruction / 0x10;
  uint8_t low = *instruction ^ 0xf;
  uint16_t address = read_register_by_name("PC");

  uint8_t vec = ((high % 0xc) * 0xf0) + (low * 0x8);

  push_to_stack(address);

  write_register_by_name("PC", vec / 0x100, vec % 0x100);

  *cpu_cycles = 4;
  *number_of_bytes = 1;
}
