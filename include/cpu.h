#ifndef CHIP_8_EMU_CPU_H
#define CHIP_8_EMU_CPU_H

#include "definitions.h"

#define MEMORY_SIZE 4096
#define VMEMORY_SIZE_X 64
#define VMEMORY_SIZE_Y 32
#define STACK_SIZE 16

struct _CPU {
    /* Memory */
    byte memory[MEMORY_SIZE];
    byte vmemory[VMEMORY_SIZE_X][VMEMORY_SIZE_Y];
    byte stack[STACK_SIZE];
    /* Register */
    byte versatile[16];
    word index;
    byte sound;
    byte delay;
    /* Pseudo Register */
    word program_counter;
    byte stack_pointer;
};
typedef struct _CPU CPU;

void cpu_reset(CPU* cpu);

#endif /* CHIP_8_EMU_CPU_H */
