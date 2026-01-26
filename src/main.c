#include <stdio.h>

#include "./sm83/alu.h"

int main() {
  printf("Hello, Gameboy\n");

  boot_cpu();

  calculate(0x23);

  return 0;
}
