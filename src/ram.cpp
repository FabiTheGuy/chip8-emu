#include <cstring>
#include "ram.hpp"


void RAM::reset() {
    std::memset(this->memory, 0, sizeof(this->memory));
    for (auto & i : this->video_memory) {
        std::memset(i, 0, sizeof(i));
    }
    this->should_draw = false;
}
