#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stddef.h>

#define CHIP8_MEM_SIZE 4096
#define CHIP8_REG_V_SIZE 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_PC_START 512

typedef uint8_t byte;
typedef uint16_t word;


typedef struct
{
    byte mem[CHIP8_MEM_SIZE];
    word reg_i;
    byte reg_v[CHIP8_REG_V_SIZE];
    byte reg_sound;
    byte reg_delay;
    word reg_pc;
    word reg_stack[CHIP8_STACK_SIZE];
    byte reg_sp;
} Chip8;

extern Chip8 chip8;


void chip8_init();

int chip8_load_rom(byte* rom, size_t rom_size);

#endif /* CHIP8_H */
