#include <stdio.h>

#include "./memory/memory.h"
#include "./sm83/alu.h"

int main() {
  printf("Hello, Gameboy\n");

  boot_cpu();

  execute_instruction(0x23);

  return 0;
}
