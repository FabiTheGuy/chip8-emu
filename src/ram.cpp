#include <cstring>
#include <stdexcept>
#include "ram.hpp"


void RAM::reset() {
    std::memset(this->memory, 0, sizeof(this->memory));
    for (auto & i : this->video_memory) {
        std::memset(i, 0, sizeof(i));
    }

    this->program_counter = 512;
    this->should_draw = false;
}

void RAM::load_program(const byte *program, const size_t program_size) {
    /* Checks if the passed program is larger than 0xDFF, if so an exception will be thrown */
    if (program_size > 0xDFF) {
        throw std::runtime_error("Program size is is larger than 0xDFF");
    }

    /* Copies data to memory starting at 0x200 */
    std::memcpy(this->memory + 0x200, program, program_size);
}

word RAM::fetch_instruction() {
    /* Gets the first byte of the instruction and shifts it 8 bits to the right */
    const word instruction_pt1 = this->memory[this->program_counter++] << 8;
    /* Gets the second byte of the instruction */
    const word instruction_pt2 = this->memory[this->program_counter++];

    /* Combines both instruction parts */
    return instruction_pt1 | instruction_pt2;
}
