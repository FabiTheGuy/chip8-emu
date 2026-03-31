#include "chip8.h"
#include "utils.h"

#include <string.h>


Chip8 chip8 = { 0 };


void chip8_init()
{
    memset(chip8.mem, 0, sizeof(byte) * CHIP8_MEM_SIZE);
    memset(chip8.reg_v, 0, sizeof(byte) * CHIP8_REG_V_SIZE);
    memset(chip8.reg_stack, 0, sizeof(word) * CHIP8_STACK_SIZE);
    memset(chip8.display, 0, sizeof(byte) * (CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_WIDTH));
    memset(chip8.keypad, 0, sizeof(byte) * CHIP8_KEYPAD_SIZE);

    memcpy(chip8.mem, sprites, sizeof(byte) * (CHIP8_SPRITE_COUNT * CHIP8_SPRITE_SIZE));

    chip8.reg_i = 0;
    chip8.reg_sound = 0;
    chip8.reg_delay = 0;
    chip8.reg_pc = CHIP8_PC_START;
    chip8.reg_sp = 0;
}

int chip8_load_rom(byte* rom, size_t rom_size)
{
    if (rom_size > CHIP8_MEM_SIZE - CHIP8_PC_START)
    {
        return 1; 
    } 

    memcpy(chip8.mem + CHIP8_PC_START, rom, sizeof(byte) * rom_size);

    return 0;
}

word chip8_fetch() 
{
    if (chip8.reg_pc >= CHIP8_MEM_SIZE - 1)
    {
        // TODO: Future error, unable to fetch memory
        return 0x0000;
    }

    word op1 = chip8.mem[chip8.reg_pc++] << 8;
    word op2 = chip8.mem[chip8.reg_pc++];

    return op1 | op2;
}

Instruction chip8_instruction(word opcode)
{
    Chip8Opcode op = chip8_decode(opcode);

    return (Instruction) {
        .opcode = op,
        .x = (opcode & 0x0F00) >> 8,
        .y = (opcode & 0x00F0) >> 4,
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
                default: return OP_UNKNOWN;
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
                default: return OP_UNKNOWN;
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
                default: return OP_UNKNOWN;
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
                default: return OP_UNKNOWN;
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
            memset(chip8.display, 0, sizeof(byte) * (CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_WIDTH));

            break;
        };
        case OP_RET: 
        {
            if (chip8.reg_sp == 0)
            {
                // TODO: Error in future
                break;
            }

            chip8.reg_pc = chip8.reg_stack[--chip8.reg_sp];

            break;
        };
        case OP_JP: 
        {
            chip8.reg_pc = instruction->nnn;

            break;
        };
        case OP_CALL: 
        {
            if (chip8.reg_sp >= CHIP8_STACK_SIZE)
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

            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = (result > 0xFF) ? 1 : 0; 
            chip8.reg_v[instruction->x] = (byte) result;

            break;
        };
        case OP_SUB: 
        {
            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = (chip8.reg_v[instruction->x] >= chip8.reg_v[instruction->y]) ? 1 : 0;
            chip8.reg_v[instruction->x] -= chip8.reg_v[instruction->y];

            break;
        };
        case OP_SHR: 
        {
            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = (chip8.reg_v[instruction->x] % 2 != 0) ? 1 : 0;
            chip8.reg_v[instruction->x] /= 2;

            break;
        };
        case OP_SUBN: 
        {
            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = (chip8.reg_v[instruction->y] >= chip8.reg_v[instruction->x]) ? 1 : 0;
            chip8.reg_v[instruction->x] = chip8.reg_v[instruction->y] - chip8.reg_v[instruction->x];

            break;
        };
        case OP_SHL: 
        {
            chip8.reg_v[CHIP8_REG_V_SIZE - 1] = (chip8.reg_v[instruction->x] & 0x80) ? 1 : 0;
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
            word new_addr = instruction->nnn + chip8.reg_v[0];

            if (new_addr >= CHIP8_MEM_SIZE)
            {
                // TODO: Error in future
                break;
            }

            chip8.reg_pc = new_addr;

            break;
        };
        case OP_RND: 
        {
            chip8.reg_v[instruction->x] = utils_random_byte() & instruction->kk;

            break;
        };
        case OP_DRW: 
        {
            byte pos_x = chip8.reg_v[instruction->x];
            byte pos_y = chip8.reg_v[instruction->y];
            byte height = instruction->n;
            byte* collision = &chip8.reg_v[CHIP8_REG_V_SIZE - 1];
            *collision = 0;

            for (byte row = 0; row < height; row++)
            {
                if (chip8.reg_i + row >= CHIP8_MEM_SIZE)
                {
                    break;
                }

                byte sprite_byte = chip8.mem[chip8.reg_i + row];

                for (byte column = 0; column < 8; column++)
                {
                    if ((sprite_byte & (0x80 >> column)) != 0)
                    {
                        byte curr_pos_x = (pos_x + column) % CHIP8_DISPLAY_WIDTH;
                        byte curr_pos_y = (pos_y + row) % CHIP8_DISPLAY_HEIGHT;

                        if (chip8.display[curr_pos_y][curr_pos_x] == 1)
                        {
                            *collision = 1; 
                        }

                        chip8.display[curr_pos_x][curr_pos_y] ^= 1;
                    }
                }
            }

            break;
        };
        case OP_SKP: 
        {
            byte key = chip8.reg_v[instruction->x];

            if (key >= CHIP8_KEYPAD_SIZE)
            {
                // TODO: Future error
                break;
            }

            if (chip8.keypad[key])
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_SKNP: 
        {
            byte key = chip8.reg_v[instruction->x];

            if (key >= CHIP8_KEYPAD_SIZE)
            {
                // TODO: Future error
                break;
            }

            if (!chip8.keypad[key])
            {
                chip8.reg_pc += 2;
            }

            break;
        };
        case OP_LD_VX_DT: 
        {
            chip8.reg_v[instruction->x] = chip8.reg_delay;

            break;
        };
        case OP_LD_VX_K: 
        {
            byte pressed = 0;

            for (byte key = 0; key < CHIP8_KEYPAD_SIZE; key++)
            {
                if (chip8.keypad[key])
                {
                    chip8.reg_v[instruction->x] = key;
                    pressed = 1;
                    break;
                }
            }

            if (!pressed)
            {
                chip8.reg_pc -= 2;
            }

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

            break;
        };
        case OP_LD_F_VX: 
        {
            byte sprite_value = chip8.reg_v[instruction->x];

            if (sprite_value >= CHIP8_SPRITE_COUNT)
            {
                // TODO: Future error, value in vx is larger than sprite
                break;
            }

            chip8.reg_i = (sprite_value * (CHIP8_SPRITE_SIZE));

            break;
        };
        case OP_LD_B_VX: 
        {
            if (chip8.reg_i + 2 >= CHIP8_MEM_SIZE)
            {
                // TODO: Error in future
                break;
            }

            byte value = chip8.reg_v[instruction->x];

            chip8.mem[chip8.reg_i] = value / 100;
            chip8.mem[chip8.reg_i + 1] = (value / 10) % 10;
            chip8.mem[chip8.reg_i + 2] = (value % 10);
            
            break;
        };
        case OP_LD_I_VX: 
        {
            byte v_x = instruction->x;

            if (chip8.reg_i + v_x >= CHIP8_MEM_SIZE)
            {
                // TODO: Future error, out of range
                break;
            }

            memcpy(chip8.mem + chip8.reg_i, chip8.reg_v, v_x + 1);

            break;
        };
        case OP_LD_VX_I: 
        {
            byte v_x = instruction->x;

            if (chip8.reg_i + v_x >= CHIP8_MEM_SIZE)
            {
                // TODO: Future error, out of range
                break;
            }

            memcpy(chip8.reg_v, chip8.mem + chip8.reg_i, v_x + 1);

            break;
        };
        case OP_UNKNOWN: 
        {
            // TODO: add global Error struct
            break;
        };
    }
}

void chip8_run_cycle()
{
    word opcode = chip8_fetch();
    Instruction instr = chip8_instruction(opcode);

    chip8_execute(&instr);
}
