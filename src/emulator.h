#ifndef CHIP8_EMU_EMULATOR_H
#define CHIP8_EMU_EMULATOR_H

#include <stdint.h>
#include <stddef.h>

#define EMU_MEM_SIZE 4096
#define EMU_REG_V_SIZE 16
#define EMU_STACK_SIZE 16
#define EMU_PC_START 512

typedef uint8_t byte;
typedef uint16_t word;

typedef struct
{
    byte mem[EMU_MEM_SIZE];
    word reg_i;
    byte reg_v[EMU_REG_V_SIZE];
    byte reg_sound;
    byte reg_delay;
    word reg_pc;
    word reg_stack[EMU_STACK_SIZE];
    byte reg_sp;
} Emulator;

extern Emulator emulator;


void emulator_init();

int emulator_load_rom(byte* rom, size_t rom_size);

#endif /* CHIP8_EMU_EMULATOR_H */
