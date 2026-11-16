#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InstructionHandler *create_instruction_handler()
{
    struct InstructionHandler *ih = malloc(sizeof(struct InstructionHandler));
    if (!ih)
        return NULL;
    memset(ih->registers, 0, sizeof ih->registers);
    ih->decode_instruction = decode_instruction;
    ih->execute_instruction = execute_instruction;
    return ih;
}

void execute_format_i(struct InstructionHandler *ih);
void execute_format_r(struct InstructionHandler *ih);

instruction_err_t decode_instruction(struct InstructionHandler *ih, int bits)
{
    printf("bits: %d\n", bits);
    printf("bits &: %d\n", bits & 0b1111111);

    switch (bits & 0b1111111)
    {
    case 0b0110011: // function type R
        printf("function type R\n");
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rs2 = (bits >> 20) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->funct7 = (bits >> 25) & 0b1111111;
        execute_format_r(ih);
        printf("end\n");
        break;

    case 0b0010011: // function type I
        printf("function type I\n");
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (bits >> 20) & 0b111111111111;
        execute_format_i(ih);
        printf("end\n");

        break;

    case 0b0100011: // function type S
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rs2 = (bits >> 20) & 0b11111;
        // ih->rd     = bits>>7  & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (((bits >> 25) & 0b1111111) << 5) | ((bits >> 7) & 0b11111);
        printf("function type S\n");
        break;

    case 0b1100011: // function type B
        ih->rs1 = bits >> 15 & 0b11111;
        ih->rs2 = bits >> 20 & 0b11111;
        // ih->rd     = bits>>7  & 0b11111;
        ih->funct3 = bits >> 12 & 0b111;
        ih->imm = (((bits >> 31) & 0b1) << 12) | (((bits >> 7) & 0b1) << 11) | (((bits >> 8) & 0b1111) << 1) | (((bits >> 25) & 0b111111) << 5);
        printf("function type B\n");
        break;

    case 0b1101111: // function type J
        // ih->rs1    = bits>>15 & 0b11111;
        // ih->rs2    = bits>>20 & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        // ih->funct3 = bits>>12 & 0b111;
        ih->imm = (((bits >> 31) & 0b1) << 20) | (((bits >> 21) & 0b1111111111) << 9) | (((bits >> 20) & 0b1) << 8) | ((bits >> 12) & 0b11111111);
        printf("function type J\n");
        break;

    case 0b0110111: // function type U
    case 0b0010111:
        // ih->rs1    = bits>>15 & 0b11111;
        // ih->rs2    = bits>>20 & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        // ih->funct3 = bits>>12 & 0b111;
        ih->imm = bits & (0b11111111111111111111) << 12;
        printf("function type U\n");
        break;

    default:
        printf("stupid ass\n");
        return UNKNOWN;
    }

    ih->registers[0] = 0; // x0 is hardwired to zero
    return OK;
}

void execute_format_r(struct InstructionHandler *ih)
{
    printf("execute_r");
    switch (ih->funct3)
    {
    case 0:
        if (ih->funct7 == 0)
        { // ADD
            int src_1 = ih->registers[ih->rs1];
            int src_2 = ih->registers[ih->rs2];

            ih->registers[ih->rd] = src_1 + src_2;
            printf("ADD x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        }
        if (ih->funct7 == 0x20)
        { // SUB
            int src_1 = ih->registers[ih->rs1];
            int src_2 = ih->registers[ih->rs2];
            
            ih->registers[ih->rd] = src_1 - src_2;
            printf("SUB x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        }
        break;

    default:
        break;
    }
}

void execute_format_i(struct InstructionHandler *ih)
{
    printf("execute_i\n");
    switch (ih->funct3)
    {
    case 0:
        // ADDI
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src + ih->imm;
        printf("ADDI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;

    default:
        break;
    }
}
instruction_err_t execute_instruction(struct InstructionHandler *ih, instruction_t instruction)
{

    return 0;
}
