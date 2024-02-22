#include "../include/cpu.hpp"

#include <cstdio>
#include <cstring>

#include "../include/flash-memory.hpp"
#include "../include/utils.hpp"
#define USED_AND_BREAK \
  {                    \
    inst_used = true;  \
    break;             \
  }

void CPU::reset(void) {
  /* Clears the memory */
  std::memset(memory, 0, 0x1000);

  /* Loads fonts into memory (0x050-0x09F)*/
  for (byte index = 0; index < 0x50; index++) {
    memory[0x50 + index] = fonts[index];
  }

  /* Clears the screen buffer */
  for (byte index = 0; index < 0x20; index++) {
    memset(scr_buff[index], 0, 0x40);
  }

  /* Clears the v-registers */
  std::memset(reg_v, 0, 0x10);

  /* Clears the stack */
  std::memset(stack, 0, 0x10);

  /* Clears the index register/program counter/stack pointer */
  reg_index = program_counter = stack_pointer = 0;

  /* Clears timers */
  delay_timer = sound_timer = 0;

  /* Clears misc */
  draw_flag = 0;
}

byte CPU::fetch(void) { return memory[0x200 + program_counter++]; }

void CPU::load_program(byte* program, word program_size) {
  for (word index = 0; index < program_size; index++) {
    memory[0x200 + index] = program[index];
  }
}

void CPU::run_cycle(void) {
  word inst = fetch() << 8 | fetch(); /* Fetches 2 Bytes merged together */
  byte x_index =
      (inst & 0x0F00) >> 8; /* Determines Bit 8-11 (in some cases x-Address) */
  byte y_index =
      (inst & 0x00F0) >> 4; /* Determines Bit 4-7 (in some cases y-Address) */
  bool inst_used = false;

  /* Checks Instructions by all 16-Bits (0x(0-F)(0-F)(0-F)(0-F)) */
  switch (inst) {
    case INS_CLS: {
      for (byte index = 0; index < 0x20; index++) {
        memset(scr_buff[index], 0, 0x40);
      }
      USED_AND_BREAK
    }
    case INS_RET: {
      program_counter = stack[stack_pointer--];
      USED_AND_BREAK
    }
  }

  /* Checks Instructions by first 4-Bits (0x(0-F)XXX) */
  switch (inst & 0xF000) {
    case INS_JP_ADDR: {
      program_counter = inst & 0x0FFF;
      USED_AND_BREAK
    }
    case INS_CALL_ADDR: {
      stack[stack_pointer++] = program_counter;
      program_counter = inst & 0x0FFF;
      USED_AND_BREAK
    }
    case INS_SE_VX_BYTE: {
      program_counter = (reg_v[x_index] == (inst & 0x00FF))
                            ? program_counter + 2
                            : program_counter;
      USED_AND_BREAK
    }
    case INS_SNE_VX_BYTE: {
      program_counter = (reg_v[x_index] != (inst & 0x00FF))
                            ? program_counter + 2
                            : program_counter;
      USED_AND_BREAK
    }
    case INS_SE_VX_VY: {
      program_counter = (reg_v[x_index] == reg_v[y_index]) ? program_counter + 2
                                                           : program_counter;
      USED_AND_BREAK
    }
    case INS_LD_VX_BYTE: {
      reg_v[x_index] = inst & 0x00FF;
      USED_AND_BREAK
    }
    case INS_ADD_VX_BYTE: {
      reg_v[x_index] += inst & 0x00FF;
      USED_AND_BREAK
    }
    case INS_SNE_VX_VY: {
      program_counter = (reg_v[x_index] != reg_v[y_index]) ? program_counter + 2
                                                           : program_counter;
      USED_AND_BREAK
    }
    case INS_LD_I_ADDR: {
      reg_index = inst & 0x0FFF;
      USED_AND_BREAK
    }
    case INS_JP_V0_ADDR: {
      program_counter = (inst & 0x0FFF) + reg_v[0x00];
      USED_AND_BREAK
    }
    case INS_RND_VX_BYTE: {
      reg_v[x_index] = random_range(0, 255) & (inst & 0x00FF);
      USED_AND_BREAK
    }
    case INS_DRW_VX_VY_NIBBLE: {
      /* TODO: after implementation of screen */
      USED_AND_BREAK
    }
  }

  /* Checks Instructions by first/last 4-Bits (0x(0-F)XX(0-F)) */
  switch (inst & 0xF00F) {
    case INS_LD_VX_VY: {
      reg_v[x_index] = reg_v[y_index];
      USED_AND_BREAK
    }
    case INS_OR_VX_VY: {
      reg_v[x_index] |= reg_v[y_index];
      USED_AND_BREAK
    }
    case INS_AND_VX_VY: {
      reg_v[x_index] &= reg_v[y_index];
      USED_AND_BREAK
    }
    case INS_XOR_VX_VY: {
      reg_v[x_index] ^= reg_v[y_index];
      USED_AND_BREAK
    }
    case INS_ADD_VX_VY: {
      word result = reg_v[x_index] + reg_v[y_index];
      reg_v[0x0F] = result > 0xFF;
      reg_v[x_index] = result & 0x00FF;
      USED_AND_BREAK
    }
    case INS_SUB_VX_VY: {
      reg_v[0x0F] = reg_v[x_index] > reg_v[y_index];
      reg_v[x_index] -= reg_v[y_index];
      USED_AND_BREAK
    }
    case INS_SHR_VX_VY: {
      reg_v[0x0F] = reg_v[x_index] & 0x01;
      reg_v[x_index] /= 2;
      USED_AND_BREAK
    }
    case INS_SUBN_VX_VY: {
      reg_v[0x0F] = reg_v[y_index] > reg_v[x_index];
      reg_v[x_index] = reg_v[y_index] - reg_v[x_index];
      USED_AND_BREAK
    }
    case INS_SHL_VX_VY: {
      reg_v[0x0F] = reg_v[x_index] >> 7;
      reg_v[x_index] *= 2;
      USED_AND_BREAK
    }
  }

  /* Checks Instructions by all Bits except of Bits 8-11 */
  switch (inst & 0xF0FF) {
    case INS_SKP_VX: {
      /* TODO: after implementation of keyboard */
      USED_AND_BREAK
    }
    case INS_SKNP_VX: {
      /* TODO: after implementation of keyboard */
      USED_AND_BREAK
    }
    case INS_LD_VX_DT: {
      reg_v[x_index] = delay_timer;
      USED_AND_BREAK
    }
    case INS_LD_VX_K: {
      /* TODO: after implementation of keyboard */
      USED_AND_BREAK
    }
    case INS_LD_DT_VX: {
      delay_timer = reg_v[x_index];
      USED_AND_BREAK
    }
    case INS_LD_ST_VX: {
      sound_timer = reg_v[x_index];
      USED_AND_BREAK
    }
    case INS_ADD_I_VX: {
      reg_index += reg_v[x_index];
      USED_AND_BREAK
    }
    case INS_LD_F_VX: {
      reg_index = 0x50 + (reg_v[x_index] * 5);
      USED_AND_BREAK
    }
    case INS_LD_B_VX: {
      byte hundrets = (reg_v[x_index] / 100) * 100;
      byte tenths = ((reg_v[x_index] - hundrets) / 10) * 10;
      byte ones = reg_v[x_index] - hundrets - tenths;

      memory[reg_index] = hundrets;
      memory[reg_index + 1] = tenths;
      memory[reg_index + 2] = ones;
      USED_AND_BREAK
    }
    case INS_LD_I_VX: {
      for (byte index = 0; index <= ((inst & 0x0F00) >> 8); index++) {
        memory[reg_index + index] = reg_v[index];
      }
      USED_AND_BREAK
    }
    case INS_LD_VX_I: {
      for (byte index = 0; index < ((inst & 0x0F00) >> 8); index++) {
        reg_v[index] = memory[reg_index + index];
      }
      USED_AND_BREAK
    }

      if (!inst_used) {
        std::fprintf(stderr, "Instruction: %04x is unknown!\n", inst);
      }
  }
}
