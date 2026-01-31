#ifndef ADDRESSING
#define ADDRESSING

#include <stdint.h>

uint8_t *read_address(uint16_t address);
void write_address(uint16_t address, uint8_t byte);

#endif
