#include <stdlib.h>
#include <string.h>

#include "../include/cpu.h"

Result create_cpu() {
    Result res;
    res.data = (CPU*) malloc(sizeof(CPU));

    if (res.data == NULL) {
        res.code = ERROR_UNABLE_TO_ALLOCATE;
        return res;
    }
    
    reset_cpu(res.data);

    res.code = OK;
    return res;
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

Result cpu_load_program(CPU* cpu, byte* data, size_t data_size) {
    Result res = { .data = NULL };
    if (data_size > 0xe000) {
        res.code = ERROR_DATA_IS_TOO_LARGE;
        return res;
    }

    memcpy(&(cpu->_memory[0x200]), data, data_size);

    res.code = OK;
    return res;
}