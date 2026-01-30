#include <stdint.h>
#include <stdlib.h>

const uint8_t *WRAM = NULL;
const uint8_t *VRAM = NULL;

void initialize_memory() {
  WRAM = malloc(8 * 1024);
  VRAM = malloc(8 * 1024);
}

uint8_t read_from_address(uint16_t address) {
  if (address < 0x0100) {
    // TODO execute boot ROM
  } else if (address < 0x4000) {
    // TODO handle ROM bank 0
  } else if (address < 0x8000) {
    // TODO handle ROM bank N
  } else if (address < 0xa000) {
    return VRAM[address - 0x8000];
  } else if (address < 0xc000) {
    // TODO handle cartridge RAM
  } else if (address < 0xe000) {
    // TODO handle RAM banking in CGB mode
    return WRAM[address - 0x8000];
  } else if (address < 0xfe00) {
    // TODO handle ECHO RAM
  } else if (address < 0xfea0) {
    // TODO handle OAM
  } else if (address < 0xff00) {
    // TODO handle prohibited area
  } else if (address < 0xff80) {
    // TODO implement IO registers
  } else if (address < 0xffff) {
    // TODO implement HRAM
  } else if (address == 0xffff) {
    // TODO implement Interrupt Enable register
  }

  return -1;
}
