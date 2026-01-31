#include "../cartridge/cartridge.h"
#include "../memory/memory.h"
#include <stdint.h>
#include <stdio.h>

uint8_t *read_address(uint16_t address) {
  printf("Reading address: 0x%04x\n", address);
  if (address < 0x0100) {
    /* uint8_t boot_rom_flag = read_IO_register(0x50); */
    /* if (boot_rom_flag == 0) { */
    /*   // TODO execute boot ROM */
    /* } */
  }
  if (address < 0x4000) {
    return read_rom(address);
  }
  if (address < 0x8000) {
    return read_rom(address);
  }
  if (address < 0xa000) {
    return read_VRAM(address - 0x8000);
  }
  if (address < 0xc000) {
    // TODO handle cartridge RAM
  }
  if (address < 0xe000) {
    // TODO handle RAM banking in CGB mode
    return read_WRAM(address - 0xc000);
  }
  if (address < 0xfe00) {
    return read_WRAM(address - 0xe000);
  }
  if (address < 0xfea0) {
    // TODO handle OAM
  }
  if (address < 0xff00) {
    // TODO handle prohibited area
  }
  if (address < 0xff80) {
    return read_IO_register(address - 0xff00);
  }
  if (address < 0xffff) {
    return read_HRAM(address - 0xff80);
  }
  if (address == 0xffff) {
    // TODO implement Interrupt Enable register
  }

  return NULL;
}

void write_address(uint16_t address, uint8_t byte) {
  printf("Reading address: 0x%04x\n", address);
  if (address < 0x0100) {
    /* uint8_t boot_rom_flag = read_IO_register(0x50); */
    /* if (boot_rom_flag == 0) { */
    /*   // TODO execute boot ROM */
    /* } */
  }
  if (address < 0x8000) {
    // you can't write to a ROM
    return;
  }
  if (address < 0xa000) {
    write_VRAM(address - 0x8000, byte);
  }
  if (address < 0xc000) {
    // TODO handle cartridge RAM
  }
  if (address < 0xe000) {
    // TODO handle RAM banking in CGB mode
    write_WRAM(address - 0xc000, byte);
  }
  if (address < 0xfe00) {
    write_WRAM(address - 0xe000, byte);
  }
  if (address < 0xfea0) {
    // TODO handle OAM
  }
  if (address < 0xff00) {
    // TODO handle prohibited area
  }
  if (address < 0xff80) {
    write_IO_register(address - 0xff00, byte);
  }
  if (address < 0xffff) {
    write_HRAM(address - 0xff80, byte);
  }
  if (address == 0xffff) {
    // TODO implement Interrupt Enable register
  }

  return;
}
