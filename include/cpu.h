#ifndef CHIP_8_EMU_CPU_H
#define CHIP_8_EMU_CPU_H

#include <stddef.h>

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

CPU* cpu_create(void);
void cpu_destroy(CPU* cpu);

/**
 * Sets the CPU back into the initial state
 * @param cpu - The CPU which will be resetted
 */
void cpu_reset(CPU* cpu);

/**
 * Loads the program data into the memory of the CPU
 * @param cpu - The CPU in which memory the data will be loaded
 * @param data - The Program which will be loaded in the memory of the CPU
 * @param data_size - The Size of the data
 * @returns 0 Successful/1 Data is to large
 */
int cpu_load_program(CPU* cpu, byte* data, size_t data_size);

/**
 * Retrieves the byte from the memory of the CPU where the Stackpointer points to.
 * The Stackpointer will be increased by one after that operation
 * @param cpu - The CPU where the byte will be retrieved
 * @returns Byte from the CPU Memory
 */
byte cpu_fetch_byte(CPU* cpu);

#endif /* CHIP_8_EMU_CPU_H */
