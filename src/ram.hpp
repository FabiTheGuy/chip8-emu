#pragma once

#include "types.hpp"


struct RAM {
    byte memory[4096] {};
    byte video_memory[32][64] {};
    word program_counter = 512;
    bool should_draw = false;

    RAM();
    void reset();
    void load_program(const byte* program, size_t program_size);
    word fetch_instruction();
};