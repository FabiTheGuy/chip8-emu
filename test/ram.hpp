#pragma once

#include <gtest/gtest.h>
#include "src/ram.hpp"
#include "src/rom.hpp"


TEST(RAMTest, Creation) {
    RAM ram;

    /* Checks that Sprites a correctly loaded (0x0000-0x004F) */
    for (size_t i = 0; i < sizeof(rom::sprites); i++) {
        ASSERT_EQ(rom::sprites[i/5][i % 5], ram.memory[i]);
    }
    /* Checks if the rest of the memory is 0 (0x0050-0x0FFF) */
    for (size_t i = sizeof(rom::sprites); i < 0x0FFF; i++) {
        ASSERT_EQ(0, ram.memory[i]);
    }

    /* Checks if the video memory is 0 */
    for (auto & i : ram.video_memory) {
        for (unsigned char j : i) {
            ASSERT_EQ(0, j);
        }
    }

    ASSERT_EQ(512, ram.program_counter);
    ASSERT_EQ(false, ram.should_draw);
}

TEST(RAMTest, Reset) {
    RAM ram;

    /* Sets the whole memory to 100 */
    for (auto & i : ram.memory) {
        i = 100;
    }
    /* Sets the whole video memory to 100 */
    for (auto & i : ram.video_memory) {
        for (unsigned char j : i) {
            j = 100;
        }
    }
    ram.program_counter = 827;
    ram.should_draw = true;

    /* Resets the RAM */
    ram.reset();

    /* Checks that Sprites a correctly loaded (0x0000-0x004F) */
    for (size_t i = 0; i < sizeof(rom::sprites); i++) {
        ASSERT_EQ(rom::sprites[i/5][i % 5], ram.memory[i]);
    }
    /* Checks if the rest of the memory is 0 (0x0050-0x0FFF) */
    for (size_t i = sizeof(rom::sprites); i < 0x0FFF; i++) {
        ASSERT_EQ(0, ram.memory[i]);
    }

    /* Checks if the video memory is 0 */
    for (auto & i : ram.video_memory) {
        for (unsigned char j : i) {
            ASSERT_EQ(0, j);
        }
    }

    ASSERT_EQ(512, ram.program_counter);
    ASSERT_EQ(false, ram.should_draw);
}