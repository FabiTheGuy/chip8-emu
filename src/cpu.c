#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "definitions.h"

CPU* cpu_create(void) {
    CPU* cpu = (CPU*) malloc(sizeof(CPU));

    if (!cpu)
        return NULL;
    cpu_reset(cpu);

    return cpu;
}

void cpu_destroy(CPU *cpu) {
    free(cpu);
}

void cpu_reset(CPU *cpu) {
    memset(cpu->memory, 0, MEMORY_SIZE);
    memset(cpu->stack, 0, STACK_SIZE);
    memset(cpu->versatile, 0, 16);
    for (int i = 0; i < VMEMORY_SIZE_Y; i++)
        memset(cpu->vmemory[i], 0, VMEMORY_SIZE_X);

    cpu->index = 0;
    cpu->sound = 0;
    cpu->delay = 0;
    cpu->program_counter = 0x200; /* 512 */
    cpu->stack_pointer = 0;
}

int cpu_load_program(CPU* cpu, byte* data, size_t data_size) {
    if (data_size > 0xFFF - 0x200)
        return 1;

    memcpy(cpu->memory + 0x200, data, data_size);

    return 0;
}

byte cpu_fetch_byte(CPU *cpu) {
    return cpu->memory[cpu->stack_pointer++];
}
