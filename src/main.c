#include <stdio.h>

#include "./cartridge/cartridge.h"
#include "./memory/memory.h"
#include "./sm83/alu.h"

int main() {
  printf("Hello, Gameboy\n");

  boot_cpu();
  initialize_memory();
  load_rom();

  /* boot_rom(); */

  execute_instruction(0x23);

  return 0;
}
