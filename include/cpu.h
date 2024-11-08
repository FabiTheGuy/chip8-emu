#ifndef CHIP_8_CPU_H
#define CHIP_8_CPU_H

#include <stddef.h>
#include "type.h"
#include "error.h"

#define MEMORY_SIZE 4096       /* Total Memory Size (4KB) */
#define VIDEO_MEMORY_WIDTH 64  /* Video Memory Width (virtual screen width) */
#define VIDEO_MEMORY_HEIGHT 32 /* Video Memory Height (virtual screen height) */
#define STACK_SIZE 16          /* Maximum call stack depth */
#define VERSATILE_REG_SIZE 16  /* Versatile Register Size */

struct _CPU {
    /* Memories */
    byte _memory[MEMORY_SIZE];                                      /* Main memory */
    byte _video_memory[VIDEO_MEMORY_HEIGHT][VIDEO_MEMORY_WIDTH];    /* Video Memory (64x32 virtual pixels) */
    byte _stack[STACK_SIZE];                                        /* Stack for storing return addresses of subroutines */

    /* Registers */
    byte _versatile[VERSATILE_REG_SIZE];                            /* General Purpose Register (V[0]-V[F]) */
    word _index;                                                    /* Index, stores memory addresses */
    byte _delay;                                                    /* Delay timer, counts down at 60Hz */
    byte _sound;                                                    /* Sound timer, counts down at 60Hz and makes sound at 0 */

    /* Pseudo Registers */
    word _program_counter;                                          /* Program counter, stores current execution address */
    byte _stack_pointer;                                            /* Stack pointer, points to the top of the stack */
};
typedef struct _CPU CPU;

/**
 * @brief Create a new CPU instance and initializes it
 * 
 * This function allocates memory for a new CPU structure and initializes it
 * to its default state by calling `reset_cpu`. If memory allocation fails,
 * the function returns NULL.
 * 
 * @return Result - A `Result` struct containing:
 *         - `data`: A pointer to the newly created CPU instance if successful, or `NULL` if the allocation fails.
 *         - `code`: A `ResultCode` indicating the status of the operation (e.g., `OK` for success, `ERROR_UNABLE_TO_ALLOCATE` for allocation failure).
 */
Result create_cpu();

/**
 * @brief Resets a given CPU instance to its initial state.
 * 
 * This function sets all the memory regions, registers, and other
 * CPU components to their default values. It ensures the CPU starts in a known
 * state by clearing memory and initializing registers.
 * 
 * @param cpu A pointer to the CPU instance to reset
 */
void reset_cpu(CPU* cpu);

/**
 * @brief Frees the memory allocated for a CPU instance.
 * 
 * This function releases the memory allocated for the CPU structure to
 * prevent memory leaks. It should be called when the CPU is no longer needed.
 * 
 * @param cpu A pointer to the CPU instance to destroy.
 */
void destroy_cpu(CPU* cpu);

/**
 * @brief Loads a program into the CPU's memory.
 * 
 * This function takes a data array representing a program and loads it into
 * the CPU's memory starting at a 0x200
 * 
 * @param cpu A pointer to the CPU instance where the program will be loaded.
 * @param data A pointer to the array of bytes representing the program data.
 * @param data_size The size of the program data in bytes.
 * 
 * @return Result - A `Result` struct containing:
 *         - `data`: A pointer which is constantly `NULL` (can be ignored!)
 *         - `code`: A `ResultCode` indicating the status of the operation (e.g., `OK` for sucess, `ERROR_DATA_IS_TOO_LARGE` if provided data is too large)
 */
Result cpu_load_program(CPU* cpu, byte* data, size_t data_size);

#endif /* CHIP_8_CPU_H */