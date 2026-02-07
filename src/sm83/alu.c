#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../addressing/addressing.h"
#include "arithmetic.h"
#include "bit_manipulation.h"
#include "carry.h"
#include "interrupts.h"
#include "jumps.h"
#include "load.h"
#include "logic.h"
#include "misc.h"
#include "registers.h"
#include "stack.h"

const int BASE_REGISTER_INDEX = 2;

void boot_cpu() { printf("CPU boot successful\n"); }

int extract_half_register_index(int low) { return low & 0x7; }

void execute_cb_prefixed_instructions(uint8_t *instruction, uint8_t *cpu_cycles,
                                      uint8_t *number_of_bytes) {
  uint8_t high = *instruction / 0x10;
  uint8_t low = *instruction & 0xf;

  switch (high) {
  case 0x0:
    if (low == 0x6) {
      rlc_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      rlc_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      rrc_HLa(instruction, cpu_cycles, number_of_bytes);
    }
    rrc_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x1:
    if (low == 0x6) {
      rl_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      rl_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      rr_HLa(instruction, cpu_cycles, number_of_bytes);
    }
    rr_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x2:
    if (low == 0x6) {
      sla_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      sla_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      sla_HLa(instruction, cpu_cycles, number_of_bytes);
    }
    sla_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x3:
    if (low == 0x6) {
      swap_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      swap_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      srl_HLa(instruction, cpu_cycles, number_of_bytes);
    }
    srl_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7:
    if (low == 0x6 || low == 0xe) {
      bit_u3_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    bit_u3_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x8:
  case 0x9:
  case 0xa:
  case 0xb:
    if (low == 0x6 || low == 0xe) {
      res_u3_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    res_u3_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
    if (low == 0x6 || low == 0xe) {
      set_u3_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    set_u3_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  default:
    break;
  }
}

int execute() {
  printf("---------------------------------------------------------------------"
         "---------CPU "
         "Cycle-------------------------------------------------------"
         "-----------------------\n");
  uint16_t PC = read_register_by_name("PC");
  uint8_t *instruction = read_address(PC);

  int high = *instruction & 0xf0;
  int low = *instruction % 0x0f;
  // CPU_CYCLES, NUMBER_OF_BYTES
  uint8_t instruction_metadata[2] = {1, 1};
  uint8_t *cpu_cycles = instruction_metadata;
  uint8_t *number_of_bytes = instruction_metadata + 1;

  printf("[ALU Context]\n");
  printf("PC: %02x\n", PC);
  printf("Instruction: 0x%02x\n", *instruction);
  printf("0000000000000\n");

  switch (high) {
  case 0x0:
    if (low == 0x0) {
      nop(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x7) {
      rlca(instruction, cpu_cycles, number_of_bytes);
    }
    if (low == 0x8) {
      ld_n16a_SP(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xf) {
      rrca(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0x1:
    if (low == 0x0) {
      stop(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      ld_r16a_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x7) {
      rla(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      jr_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      ld_A_r16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xf) {
      rra(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0x2:
    if (low == 0x0) {
      jr_NZ_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      ld_HLIa_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x4) {
      inc_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x5) {
      dec_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x6) {
      ld_r8_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x7) {
      daa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      ld_A_HLIa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      cpl(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0x3:
    if (low == 0x0) {
      jr_NC_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x1) {
      ld_r16_n16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      ld_HLDa_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x3) {
      inc_r16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x4) {
      inc_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x5) {
      dec_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x6) {
      ld_HLa_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x7) {
      scf(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      jr_Z_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x9) {
      add_HL_r16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      ld_A_HLDa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xb) {
      dec_r16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xc) {
      inc_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xd) {
      dec_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      ld_r8_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xf) {
      ccf(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0x4:
  case 0x5:
  case 0x6:
    if (low == 0x6 || low == 0xe) {
      ld_r8_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0x7:
    if (low == 0x6) {
      halt(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      ld_r8_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    ld_r8_r8(instruction, instruction_metadata, instruction_metadata + 1);
    break;
  case 0x8:
    if (low == 0x6) {
      add_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      add_A_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      adc_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    adc_A_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0x9:
    if (low == 0x6) {
      sub_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      sub_A_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      sbc_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    sbc_A_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0xa:
    if (low == 0x6) {
      and_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      and_A_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      xor_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    xor_A_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0xb:
    if (low == 0x6) {
      or_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low < 0x8) {
      or_A_r8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      cp_A_HLa(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    cp_A_r8(instruction, cpu_cycles, number_of_bytes);
    break;
  case 0xc:
    if (low == 0x0) {
      ret_NZ(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      jp_NZ_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x3) {
      jp_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x4) {
      call_NZ_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x6) {
      add_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      ret_Z(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x9) {
      ret(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      jp_Z_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xb) {
      // TODO bitwise ops here
      break;
    }
    if (low == 0xc) {
      call_Z_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xd) {
      call_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xe) {
      adc_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    break;
  case 0xd:
    if (low == 0x0) {
      ret_NC(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      jp_NC_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x3) {
      break;
    }
    if (low == 0x4) {
      call_NC_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x6) {
      sub_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      ret_C(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x9) {
      reti(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      jp_C_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xb) {
      break;
    }
    if (low == 0xc) {
      call_C_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xd) {
      break;
    }
    if (low == 0xe) {
      sbc_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    break;
  case 0xe:
    if (low == 0x0) {
      ldh_n8a_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      ldh_Ca_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x3) {
      break;
    }
    if (low == 0x4) {
      break;
    }
    if (low == 0x6) {
      and_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      add_SP_e8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x9) {
      jp_HL(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      ld_n16a_A(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xb) {
      break;
    }
    if (low == 0xc) {
      break;
    }
    if (low == 0xd) {
      break;
    }
    if (low == 0xe) {
      xor_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  case 0xf:
    if (low == 0x0) {
      ldh_A_n8a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x1) {
      pop_r16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x2) {
      ldh_A_Ca(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x3) {
      di(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x4) {
      break;
    }
    if (low == 0x5) {
      push_r16(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x6) {
      or_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x7) {
      rst(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x8) {
      ld_HL_SPe8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0x9) {
      ld_SP_HL(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xa) {
      ld_A_n16a(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xb) {
      ei(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xc) {
      break;
    }
    if (low == 0xd) {
      break;
    }
    if (low == 0xe) {
      cp_A_n8(instruction, cpu_cycles, number_of_bytes);
      break;
    }
    if (low == 0xf) {
      rst(instruction, cpu_cycles, number_of_bytes);
      break;
    }
  default:
    break;
  }

  // TODO don't update PC if updated by instruction already no?
  PC = read_register_by_name("PC");
  PC += *number_of_bytes;
  write_register_by_name("PC", PC / 0x100, PC % 0x100);

  return *cpu_cycles;
}
