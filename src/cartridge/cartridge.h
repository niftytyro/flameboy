#ifndef CARTRIDGE
#define CARTRIDGE

#include <stdint.h>

int load_rom(char *path);

uint8_t *read_cgb_flag();

uint8_t *read_cartridge_type();

uint8_t *read_rom_size();

uint8_t *read_ram_size();

uint8_t *read_header_checksum();

uint8_t *read_rom(uint16_t address);

void print_rom_contents();

#endif
