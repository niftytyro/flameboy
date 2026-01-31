#ifndef CARTRIDGE
#define CARTRIDGE

#include <stdint.h>

int load_rom(char *path);

const uint8_t *read_cgb_flag();

const uint8_t *read_cartridge_type();

const uint8_t *read_rom_size();

const uint8_t *read_ram_size();

const uint8_t *read_header_checksum();

const uint8_t *read_rom(uint16_t address);

void print_rom_contents();

#endif
