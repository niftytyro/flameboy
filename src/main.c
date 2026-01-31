#include <stdint.h>
#include <stdio.h>

#include "./addressing/addressing.h"
#include "./cartridge/cartridge.h"
#include "./memory/memory.h"
#include "./sm83/alu.h"

int main() {
  int status;
  char *rom_path = "./roms/test.gb";

  printf("Hello, Flameboy\n");

  printf("Loading the ROM: %s\n", rom_path);

  boot_cpu();

  status = load_rom(rom_path);

  if (status != 0) {
    printf("Something went wrong while loading the ROM. Exiting with status "
           "code %d",
           status);
    return -1;
  }

  const uint8_t *cartridge_ram_size = read_ram_size();
  printf("Cartridge RAM size: %d\n", *cartridge_ram_size);

  initialize_memory(*cartridge_ram_size);

  /* boot_rom(); */

  int executions = 0;
  while (executions < 50) {
    execute();
    executions++;
  }

  return 0;
}
