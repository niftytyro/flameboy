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

uint8_t read_address(uint16_t address) {
  if (address < 0x0100) {
    if (IO_REGISTERS[0x50] == 0) {
      // TODO execute boot ROM
    }
  }
  if (address < 0x4000) {
    return read_rom(address);
  }
  if (address < 0x8000) {
    return read_rom(address);
  }
  if (address < 0xa000) {
    return VRAM[address - 0x8000];
  }
  if (address < 0xc000) {
    // TODO handle cartridge RAM
  }
  if (address < 0xe000) {
    // TODO handle RAM banking in CGB mode
    return WRAM[address - 0xc000];
  }
  if (address < 0xfe00) {
    return WRAM[address - 0xe000];
  }
  if (address < 0xfea0) {
    // TODO handle OAM
  }
  if (address < 0xff00) {
    // TODO handle prohibited area
  }
  if (address < 0xff80) {
    return IO_REGISTERS[address - 0xff00];
  }
  if (address < 0xffff) {
    return HRAM[address - 0xff80];
  }
  if (address == 0xffff) {
    // TODO implement Interrupt Enable register
  }

  return -1;
}
