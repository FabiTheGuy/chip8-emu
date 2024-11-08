#include <stdlib.h>
#include <string.h>

#include "../include/cpu.h"


CPU* create_cpu() {
    CPU* cpu = (CPU*) malloc(sizeof(CPU));

    if (cpu == NULL)
        return NULL;

    reset_cpu(cpu);

    return cpu;
}

void reset_cpu(CPU* cpu) {
    memset(cpu->_memory, 0, MEMORY_SIZE);
    for (byte i = 0; i < VIDEO_MEMORY_HEIGHT; i++)
        memset(cpu->_video_memory[i], 0, VIDEO_MEMORY_WIDTH);
    memset(cpu->_stack, 0, STACK_SIZE);
    memset(cpu->_versatile, 0, VERSATILE_REG_SIZE);

    cpu->_index = 0;
    cpu->_delay = 0;
    cpu->_sound = 0;
    cpu->_program_counter = 0;
    cpu->_stack_pointer = 0;
}

void destroy_cpu(CPU* cpu) {
    free(cpu);
}