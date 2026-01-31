#include <stdint.h>
#include <stdlib.h>

#include "../cartridge/cartridge.h"

const uint8_t *HRAM = NULL;
const uint8_t *WRAM = NULL;
const uint8_t *VRAM = NULL;
const uint8_t *IO_REGISTERS = NULL;
const uint8_t *EXTERNAL_RAM = NULL;

void initialize_memory(uint8_t cartridge_ram_size) {
  WRAM = calloc(8 * 1024, sizeof(uint8_t));
  VRAM = calloc(8 * 1024, sizeof(uint8_t));
  HRAM = calloc(128, sizeof(uint8_t));
  IO_REGISTERS = calloc(0xff7f - 0xff00 + 1, sizeof(uint8_t));
  EXTERNAL_RAM = calloc(cartridge_ram_size, sizeof(uint8_t));
}

uint8_t read_WRAM(uint16_t address) { return WRAM[address]; }

uint8_t read_VRAM(uint16_t address) { return VRAM[address]; }

uint8_t read_HRAM(uint16_t address) { return VRAM[address]; }

uint8_t read_IO_register(uint16_t address) { return IO_REGISTERS[address]; }
