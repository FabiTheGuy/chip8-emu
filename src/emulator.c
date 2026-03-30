#include "emulator.h"

#include <string.h>


Emulator emulator = { 0 };


void emulator_init()
{
    memset(emulator.mem, 0, sizeof(byte) * EMU_MEM_SIZE);
    memset(emulator.reg_v, 0, sizeof(byte) * EMU_REG_V_SIZE);
    memset(emulator.reg_stack, 0, sizeof(word) * EMU_STACK_SIZE);

    emulator.reg_i = 0;
    emulator.reg_sound = 0;
    emulator.reg_delay = 0;
    emulator.reg_pc = EMU_PC_START;
    emulator.reg_sp = 0;
}

int emulator_load_rom(byte* rom, size_t rom_size)
{
    if (EMU_MEM_SIZE - EMU_PC_START < rom_size)
    {
        // TODO: might improve calcs
        return 1; 
    }

    memcpy(emulator.mem + EMU_PC_START, rom, sizeof(byte) * rom_size);

    return 0;
}
