#include <stdint.h>
#include <stdio.h>

#include "./addressing/addressing.h"
#include "./cartridge/cartridge.h"
#include "./memory/memory.h"
#include "./sm83/alu.h"

int main() {
  int status;
  char *rom_path = "./roms/load.gb";

  printf("Hello, Flameboy\n");

  printf("Booting CPU: %s\n", rom_path);

  boot_cpu();

  printf("Loading the ROM: %s\n", rom_path);

  status = load_rom(rom_path);

  if (status != 0) {
    printf("Something went wrong while loading the ROM. Exiting with status "
           "code %d",
           status);
    return -1;
  }

  uint8_t *cartridge_ram_size = read_ram_size();
  printf("Cartridge RAM size: %d\n", *cartridge_ram_size);

  printf("Initializing memory\n");

  initialize_memory(*cartridge_ram_size);

  /* boot_rom(); */

  printf("Starting execution.\n");

  int executions = 0;
  while (executions < 0x180 - 0x150) {
    execute();
    executions++;
  }

  print_cpu_state();

  return 0;
}
