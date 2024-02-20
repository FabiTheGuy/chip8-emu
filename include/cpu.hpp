#pragma once

#include "definitions.hpp"

class CPU {
 private:
  byte memory[0x1000];
  byte scr_buff[0x20][0x40];
  byte reg_v[0x10];
  word reg_index;
  word program_counter;
  word stack[0x10];
  word stack_pointer;
  byte delay_timer;
  byte sound_timer;
  byte draw_flag;

 private:
  byte fetch(void);

 public:
  void reset(void);
  void load_program(byte* program, word program_size);
};
