#pragma once

#include "types.hpp"


struct RAM {
    byte memory[4096];
    byte video_memory[32][64];
    bool should_draw;

    RAM();
    void reset();
    void load_program(byte* program, size_t program_size);
    word fetch_instruction();
};