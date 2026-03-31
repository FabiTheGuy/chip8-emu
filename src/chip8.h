#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stddef.h>

#define CHIP8_MEM_SIZE 4096
#define CHIP8_REG_V_SIZE 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_PC_START 512
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_KEYPAD_SIZE 16
#define CHIP8_SPRITE_COUNT 16
#define CHIP8_SPRITE_SIZE 5

typedef uint8_t byte;
typedef uint16_t word;

typedef enum 
{
    OP_CLS,
    OP_RET,
    OP_JP,
    OP_CALL,
    OP_SE_VX_BYTE,
    OP_SNE_VX_BYTE,
    OP_SE_VX_VY,
    OP_LD_VX_BYTE,
    OP_ADD_VX_BYTE,
    OP_LD_VX_VY,
    OP_OR,
    OP_AND,
    OP_XOR,
    OP_ADD,
    OP_SUB,
    OP_SHR,
    OP_SUBN,
    OP_SHL,
    OP_SNE_VX_VY,
    OP_LD_I,
    OP_JP_V0,
    OP_RND,
    OP_DRW,
    OP_SKP,
    OP_SKNP,
    OP_LD_VX_DT,
    OP_LD_VX_K,
    OP_LD_DT_VX,
    OP_LD_ST_VX,
    OP_ADD_I_VX,
    OP_LD_F_VX,
    OP_LD_B_VX,
    OP_LD_I_VX,
    OP_LD_VX_I,

    OP_UNKNOWN,
} Chip8Opcode;

typedef struct
{
    Chip8Opcode opcode;
    byte x;
    byte y;
    byte n;
    byte kk;
    word nnn;
} Instruction;

static byte sprites[CHIP8_SPRITE_COUNT][CHIP8_SPRITE_SIZE] = {
    { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, /* 0 */
    { 0x20, 0x60, 0x20, 0x20, 0x70 }, /* 1 */
    { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, /* 2 */
    { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, /* 3 */
    { 0x90, 0x90, 0xF0, 0x10, 0x10 }, /* 4 */
    { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }, /* 5 */
    { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, /* 6 */
    { 0xF0, 0x10, 0x20, 0x40, 0x40 }, /* 7 */
    { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, /* 8 */
    { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, /* 9 */
    { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, /* A */
    { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, /* B */
    { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, /* C */
    { 0xE0, 0x90, 0x90, 0x90, 0xE0 }, /* D */
    { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, /* E */
    { 0xF0, 0x80, 0xF0, 0x80, 0x80 }, /* F */
};


typedef struct
{
    byte mem[CHIP8_MEM_SIZE];
    word reg_i;
    byte reg_v[CHIP8_REG_V_SIZE];
    byte reg_sound;
    byte reg_delay;
    word reg_pc;
    word reg_stack[CHIP8_STACK_SIZE];
    byte reg_sp;
    byte display[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
    byte keypad[CHIP8_KEYPAD_SIZE];
} Chip8;

extern Chip8 chip8;


void chip8_init();

int chip8_load_rom(byte* rom, size_t rom_size);

word chip8_fetch();

Instruction chip8_instruction(word opcode);

Chip8Opcode chip8_decode(word opcode);

void chip8_execute(Instruction* instruction);

void chip8_run_cycle();

#endif /* CHIP8_H */
