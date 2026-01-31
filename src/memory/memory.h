#ifndef MEMORY
#define MEMORY

#include <stdint.h>

void initialize_memory(uint8_t cartridge_ram_size);

uint8_t *read_WRAM(uint16_t address);
uint8_t *read_VRAM(uint16_t address);
uint8_t *read_HRAM(uint16_t address);
uint8_t *read_IO_register(uint16_t address);


void write_WRAM(uint16_t address, uint8_t byte);
void write_VRAM(uint16_t address, uint8_t byte);
void write_HRAM(uint16_t address, uint8_t byte);
void write_IO_register(uint16_t address, uint8_t byte);

#endif
