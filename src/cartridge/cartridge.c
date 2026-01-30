#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../memory/memory.h"

uint8_t *ROM_BANK0 = NULL;

void load_rom() { ROM_BANK0 = malloc(16 * 1024); }

uint8_t read_cgb_flag() { return read_address(0x143); }

uint8_t read_cartridge_type() { return read_address(0x147); }

uint8_t read_rom_size() { return read_address(0x148); }

uint8_t read_ram_size() { return read_address(0x149); }

uint8_t read_header_checksum() { return read_address(0x14d); }

uint8_t read_rom(uint16_t address) {
  if (address < 0x4000) {
    return ROM_BANK0[address];
  }
  // TODO implement banked ROM
  return 0;
}
