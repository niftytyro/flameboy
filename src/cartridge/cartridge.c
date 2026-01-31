#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../addressing/addressing.h"
#include "cartridge.h"

const int sizeof_ROM_BANK0 = 16 * 1024;
uint8_t *ROM_BANK0 = NULL;

int load_rom(char *path) {
  ROM_BANK0 = calloc(sizeof_ROM_BANK0, sizeof(uint8_t));
  FILE *fp;
  int bytes_read = 0;
  uint8_t byte;

  fp = fopen(path, "rb");

  printf("Opened the file.\n");

  if (fp == NULL) {
    return -1;
  }

  while (fread(&byte, sizeof(byte), 1, fp) > 0 && bytes_read < 16 * 1024) {
    ROM_BANK0[bytes_read] = byte;
    /* printf("%02x ", byte); */
    bytes_read += 1;
  }

  printf("\nROM loaded. Bytes read: %d\n", bytes_read);

  print_rom_contents();

  fclose(fp);
  return 0;
}

uint8_t *read_cgb_flag() { return read_address(0x143); }

uint8_t *read_cartridge_type() { return read_address(0x147); }

uint8_t *read_rom_size() { return read_address(0x148); }

uint8_t *read_ram_size() {
  printf("Reading RAM size at 0x%04x\n", 0x149);
  return read_address(0x149);
}

uint8_t *read_header_checksum() { return read_address(0x14d); }

void print_rom_contents() {
  printf("=============================== Printing ROM Contents "
         "===============================\n");
  int i = 0;
  while (i < sizeof_ROM_BANK0) {
    printf("%02x ", ROM_BANK0[i]);
    i++;
  }
  printf("\n======================================================"
         "===============================\n");
}

uint8_t *read_rom(uint16_t address) {
  if (address < 0x4000) {
    printf("ROM_BANK0[$%04x]: 0x%02x\n", address, ROM_BANK0[address]);
    return &ROM_BANK0[address];
  }
  // TODO implement banked ROM
  return 0;
}
