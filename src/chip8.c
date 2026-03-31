#include "chip8.h"
#include "utils.h"

#include <string.h>


Chip8 chip8 = { 0 };


void chip8_init()
{
    memset(chip8.mem, 0, sizeof(byte) * CHIP8_MEM_SIZE);
    memset(chip8.reg_v, 0, sizeof(byte) * CHIP8_REG_V_SIZE);
    memset(chip8.reg_stack, 0, sizeof(word) * CHIP8_STACK_SIZE);

    chip8.reg_i = 0;
    chip8.reg_sound = 0;
    chip8.reg_delay = 0;
    chip8.reg_pc = CHIP8_PC_START;
    chip8.reg_sp = 0;

    // TODO: load sprite data
}

int chip8_load_rom(byte* rom, size_t rom_size)
{
    if (CHIP8_MEM_SIZE - (CHIP8_PC_START - 1) < rom_size)
    {
        return 1; 
    } 

    memcpy(chip8.mem + CHIP8_PC_START, rom, sizeof(byte) * rom_size);

    return 0;
}

word chip8_fetch() 
{
    word op1 = chip8.mem[chip8.reg_pc++] << 8;
    word op2 = chip8.mem[chip8.reg_pc++];

    return op1 | op2;
}

Instruction chip8_instruction(word opcode)
{
    Chip8Opcode op = chip8_decode(opcode);

    return (Instruction) {
        .opcode = op,
        .x = 8 >> (opcode & 0x0F00),
        .y = 4 >> (opcode & 0x00F0),
        .n = opcode & 0x000F,
        .kk = opcode & 0x00FF,
        .nnn = opcode & 0x0FFF,
    };
}

Chip8Opcode chip8_decode(word opcode) {
    switch (opcode & 0xF000) {
        case 0x0000:
        {
            switch (opcode) 
            {
                case 0x00E0: return OP_CLS;
                case 0x00EE: return OP_RET;
            }
        }

        case 0x1000: return OP_JP;
        case 0x2000: return OP_CALL;
        case 0x3000: return OP_SE_VX_BYTE;
        case 0x4000: return OP_SNE_VX_BYTE;

        case 0x5000:
        {
            if ((opcode & 0x000F) == 0x0)
            {
                return OP_SE_VX_VY;
            }
            return OP_UNKNOWN;
        }

        case 0x6000: return OP_LD_VX_BYTE;
        case 0x7000: return OP_ADD_VX_BYTE;

        case 0x8000:
        {
            switch (opcode & 0x000F)
            {
                case 0x0: return OP_LD_VX_VY;
                case 0x1: return OP_OR;
                case 0x2: return OP_AND;
                case 0x3: return OP_XOR;
                case 0x4: return OP_ADD;
                case 0x5: return OP_SUB;
                case 0x6: return OP_SHR;
                case 0x7: return OP_SUBN;
                case 0xE: return OP_SHL;
                default:  return OP_UNKNOWN;
            }
        }

        case 0x9000:
        {
            if ((opcode & 0x000F) == 0x0)
            {
                return OP_SNE_VX_VY;
            }
            return OP_UNKNOWN;
        }

        case 0xA000: return OP_LD_I;
        case 0xB000: return OP_JP_V0;
        case 0xC000: return OP_RND;
        case 0xD000: return OP_DRW;

        case 0xE000:
        {
            switch (opcode & 0x00FF) 
            {
                case 0x9E: return OP_SKP;
                case 0xA1: return OP_SKNP;
                default:   return OP_UNKNOWN;
            }
        }

        case 0xF000:
        {
            switch (opcode & 0x00FF) 
            {
                case 0x07: return OP_LD_VX_DT;
                case 0x0A: return OP_LD_VX_K;
                case 0x15: return OP_LD_DT_VX;
                case 0x18: return OP_LD_ST_VX;
                case 0x1E: return OP_ADD_I_VX;
                case 0x29: return OP_LD_F_VX;
                case 0x33: return OP_LD_B_VX;
                case 0x55: return OP_LD_I_VX;
                case 0x65: return OP_LD_VX_I;
                default:   return OP_UNKNOWN;
            }
        }
    }

    return OP_UNKNOWN;
}

void chip8_execute(Instruction* instruction)
{
    // TODO: Add validate instruction (checks if x, y < CHIP8_REG_V_SIZE, ....)


    switch (instruction->opcode)
    {
        case OP_CLS: 
        {
            // TODO: need "V-RAM"
            break;
        };
        case OP_RET: 
        {
            if (chip8.reg_sp == 0)
            {
                // TODO: Error in future
            }

            chip8.reg_pc = chip8.reg_stack[chip8.reg_sp--];

            break;
        };
        case OP_JP: 
        {
            chip8.reg_pc = instruction->nnn;

            break;
        };
        case OP_CALL: 
        {
            if (chip8.reg_sp == CHIP8_STACK_SIZE - 1)
            {
                // TODO: Error in future
            }

            chip8.reg_stack[chip8.reg_sp++] = chip8.reg_pc;
            chip8.reg_pc = instruction->nnn;

            break;
        };
        case OP_SE_VX_BYTE: 
        {
            if (chip8.reg_v[instruction->x] == instruction->kk)
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_SNE_VX_BYTE: 
        {
            if (chip8.reg_v[instruction->x] != instruction->kk)
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_SE_VX_VY: 
        {
            if (chip8.reg_v[instruction->x] == chip8.reg_v[instruction->y])
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_LD_VX_BYTE: 
        {
            chip8.reg_v[instruction->x] = instruction->kk;

            break;
        };
        case OP_ADD_VX_BYTE: 
        {
            chip8.reg_v[instruction->x] += instruction->kk;

            break;
        };
        case OP_LD_VX_VY: 
        {
            chip8.reg_v[instruction->x] = chip8.reg_v[instruction->y];

            break;
        };
        case OP_OR: 
        {
            chip8.reg_v[instruction->x] |= chip8.reg_v[instruction->y];

            break;
        };
        case OP_AND: 
        {
            chip8.reg_v[instruction->x] &= chip8.reg_v[instruction->y];
            
            break;
        };
        case OP_XOR: 
        {
            chip8.reg_v[instruction->x] ^= chip8.reg_v[instruction->y];

            break;
        };
        case OP_ADD: 
        {
            word result = chip8.reg_v[instruction->x] + chip8.reg_v[instruction->y];

            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = 1;
            chip8.reg_v[instruction->x] = (byte) result;

            break;
        };
        case OP_SUB: 
        {
            if (chip8.reg_v[instruction->x] > chip8.reg_v[instruction->y])
            {
                chip8.reg_v[CHIP8_REG_V_SIZE - 1] = 1;
            }

            chip8.reg_v[instruction->x] -= chip8.reg_v[instruction->y];

            break;
        };
        case OP_SHR: 
        {
            if (chip8.reg_v[instruction->x] % 2)
            {
                chip8.reg_v[CHIP8_REG_V_SIZE - 1] = 1;
            }

            chip8.reg_v[instruction->x] /= 2;

            break;
        };
        case OP_SUBN: 
        {
            if (chip8.reg_v[instruction->y] > chip8.reg_v[instruction->x])
            {
                chip8.reg_v[CHIP8_REG_V_SIZE - 1] = 1;
            }

            chip8.reg_v[instruction->x] = chip8.reg_v[instruction->y] - chip8.reg_v[instruction->x];

            break;
        };
        case OP_SHL: 
        {
            if (chip8.reg_v[instruction->x] & 0x80) 
            {
                chip8.reg_v[CHIP8_REG_V_SIZE - 1] = 1;
            }

            chip8.reg_v[instruction->x] *= 2;

            break;
        };
        case OP_SNE_VX_VY: 
        {
            if (chip8.reg_v[instruction->x] != chip8.reg_v[instruction->y])
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_LD_I: 
        {
            chip8.reg_i = instruction->nnn;

            break;
        };
        case OP_JP_V0: 
        {
            // TODO: might check if new addr is in range
            chip8.reg_pc = instruction->nnn + chip8.reg_v[0];

            break;
        };
        case OP_RND: 
        {
            chip8.reg_v[instruction->x] = utils_random_byte() & instruction->kk;

            break;
        };
        case OP_DRW: 
        {
            // TODO: need V-RAM
            break;
        };
        case OP_SKP: 
        {
            // TODO: need keypad
            break;
        };
        case OP_SKNP: 
        {
            // TODO: need keypad
            break;
        };
        case OP_LD_VX_DT: 
        {
            chip8.reg_v[instruction->x] = chip8.reg_delay;

            break;
        };
        case OP_LD_VX_K: 
        {
            // TODO: need keypad
            break;
        };
        case OP_LD_DT_VX: 
        {
            chip8.reg_delay = chip8.reg_v[instruction->x];

            break;
        };
        case OP_LD_ST_VX: 
        {
            chip8.reg_sound = chip8.reg_v[instruction->x];

            break;
        };
        case OP_ADD_I_VX: 
        {
            chip8.reg_i += chip8.reg_v[instruction->x];
        };
        case OP_LD_F_VX: 
        {
            // TODO: need sprites
        };
        case OP_LD_B_VX: 
        {
            
        };
        case OP_LD_I_VX: 
        {
            // TODO: check if I is in range when trying to copy
        };
        case OP_LD_VX_I: 
        {
            // TODO: check if I is in range when trying to read
        };
        case OP_UNKNOWN: 
        {
            // TODO: add global Error struct
        };
    }
}

void chip8_run_cycle()
{
    word opcode = chip8_fetch();
    Instruction instr = chip8_instruction(opcode);

    chip8_execute(&instr);
}
