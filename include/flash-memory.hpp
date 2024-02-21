#pragma once

#include "definitions.hpp"

/* Instructions */
#define INS_EMPTY            0x0000 /* empty instruction */
#define INS_CLS              0x00E0 /* clears the display */
#define INS_RET              0x00EE /* returns from subroutine */
#define INS_JP_ADDR          0x1000 /* (1NNN) jumps to location NNN */
#define INS_CALL_ADDR        0x2000 /* (2NNN) calls subroutine at NNN */
#define INS_SE_VX_BYTE       0x3000 /* skips next instruction if reg_v[x] == Byte */
#define INS_SNE_VX_BYTE      0x4000 /* skips next instruction if reg_v[x] != Byte */
#define INS_SE_VX_VY         0x5000 /* skips next instruction if reg_v[x] == reg_v[y] */
#define INS_LD_VX_BYTE       0x6000 /* sets reg_v[x] = kk */
#define INS_ADD_VX_BYTE      0x7000 /* sets reg_[v] += kk */
#define INS_LD_VX_VY         0x8000 /* sets reg_v[x] = reg_v[y] */
#define INS_OR_VX_VY         0x8001 /* sets reg_v[x] |= reg_v[y]*/
#define INS_AND_VX_VY        0x8002 /* sets reg_v[x] &= reg_v[y] */
#define INS_XOR_VX_VY        0x8003 /* sets reg_v[x] ^= reg_v[y] */
#define INS_ADD_VX_VY        0x8004 /* sets reg_v[x] += reg_v[y]; reg_v[0x0F] = 1 if larger than 255 */
#define INS_SUB_VX_VY        0x8005 /* sets reg_v[x] -= reg_v[y]; reg_v[0x0F] = 1 if reg_v[x] > reg_v[y] */
#define INS_SHR_VX_VY        0x8006 /* sets reg_v[x] / 2; reg_v[0x0F] = 1 if reg_v[x] % 2 = 1 */
#define INS_SUBN_VX_VY       0x8009 /* sets reg_v[x] = reg_v[y] - reg_v[x]; reg_v[0x0F] = 1 if reg_v[y] > reg_v[x]*/
#define INS_SHL_VX_VY        0x800E /* sets reg_v[x] *= 2; reg_v[0x0F] = 1 if reg_v[x] & 0x80 = 1 */ 
#define INS_SNE_VX_VY        0x9000 /* skips instruction if reg_v[x] != reg_v[y] */
#define INS_LD_I_ADDR        0xA000 /* sets reg_index = NNN */
#define INS_JP_V0_ADDR       0xB000 /* jumps to location of reg_v[0x00] + NNN */
#define INS_RND_VX_BYTE      0xC000 /* sets reg_v[x] = rand() & kk */
#define INS_DRW_VX_VY_NIBBLE 0xD000 /* draws n-bytes sprite from memory at reg_index to screen */
#define INS_SKP_VX           0xE09E /* skips instruction if key with reg_v[x] is down */
#define INS_SKNP_VX          0xE0A1 /* skips instruction if key with reg_v[x] is not down */
#define INS_LD_VX_DT         0xF007 /* sets reg_v[x] = delay_timer */
#define INS_LD_VX_K          0xF00A /* waits for a key and sets reg_v[x] = key */
#define INS_LD_DT_VX         0xF015 /* sets delay_timer to reg_v[x] */
#define INS_LD_ST_VX         0xF018 /* sets sound_timer to reg_v[x] */
#define INS_ADD_I_VX         0xF01E /* sets reg_index += reg_v[x] */
#define INS_LD_F_VX          0xF029 /* sets I location of the sprite for reg_v[x] */
#define INS_LD_B_VX          0xF033 /* stores BCD representation of reg_v[x] in memory location reg_index, reg_index + 1, reg_index + 2*/
#define INS_LD_I_VX          0xF055 /* stores reg_v[0x00] - reg_v[0x0F] into memory at location reg_index */
#define INS_LD_VX_I          0xF065 /* writes from memory at location reg_index to reg_v[0x00] - reg_v[0x0F] */

static const byte fonts[0x50] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};
