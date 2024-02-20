#include "../include/cpu.hpp"

#include <cstdio>
#include <cstring>

#include "../include/flash-memory.hpp"

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

  /* Clears miscs */
  draw_flag = 0;
}

byte CPU::fetch(void) { return memory[0x200 + program_counter++]; }

void CPU::load_program(byte* program, word program_size) {
  for (word index = 0; index < program_size; index++) {
    memory[0x200 + index] = program[index];
  }
}
