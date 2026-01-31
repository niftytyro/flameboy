#include <stdint.h>
#include <stdlib.h>

#include "../cartridge/cartridge.h"

const uint8_t *HRAM = NULL;
const uint8_t *WRAM = NULL;
const uint8_t *VRAM = NULL;
const uint8_t *IO_REGISTERS = NULL;

void initialize_memory() {
  WRAM = malloc(8 * 1024);
  VRAM = malloc(8 * 1024);
  HRAM = malloc(128);
  IO_REGISTERS = malloc(0xff7f - 0xff00 + 1);
}

uint8_t read_WRAM(uint16_t address) { return WRAM[address]; }

uint8_t read_VRAM(uint16_t address) { return VRAM[address]; }

uint8_t read_HRAM(uint16_t address) { return VRAM[address]; }

uint8_t read_IO_register(uint16_t address) { return IO_REGISTERS[address]; }
