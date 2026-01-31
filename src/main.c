#include <stdio.h>

#include "./cartridge/cartridge.h"
#include "./memory/memory.h"
#include "./sm83/alu.h"

int main() {
  printf("Hello, Gameboy\n");

  char *rom_path = "../roms/test.gb";

  boot_cpu();
  load_rom(rom_path);
  uint8_t cartridge_ram_size = read_ram_size();
  initialize_memory(cartridge_ram_size);

  /* boot_rom(); */

  while (1) {
    int cpu_cycles = execute();
    if (cpu_cycles < 0) {
      break;
    }
  }

  return 0;
}
