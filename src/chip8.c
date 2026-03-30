#include "chip8.h"

#include <string.h>


Chip8 chip8 = { 0 };


void emulator_init()
{
    memset(chip8.mem, 0, sizeof(byte) * CHIP8_MEM_SIZE);
    memset(chip8.reg_v, 0, sizeof(byte) * CHIP8_REG_V_SIZE);
    memset(chip8.reg_stack, 0, sizeof(word) * CHIP8_STACK_SIZE);

    chip8.reg_i = 0;
    chip8.reg_sound = 0;
    chip8.reg_delay = 0;
    chip8.reg_pc = CHIP8_PC_START;
    chip8.reg_sp = 0;
}

int emulator_load_rom(byte* rom, size_t rom_size)
{
    if (CHIP8_MEM_SIZE - CHIP8_PC_START < rom_size)
    {
        // TODO: might improve calcs
        return 1; 
    }

    memcpy(chip8.mem + CHIP8_PC_START, rom, sizeof(byte) * rom_size);

    return 0;
}
