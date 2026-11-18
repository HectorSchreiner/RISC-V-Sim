#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct InstructionHandler *create_instruction_handler()
{
    struct InstructionHandler *ih = malloc(sizeof(struct InstructionHandler));
    if (!ih)
        return NULL;
    memset(ih->registers, 0, sizeof ih->registers);
    ih->halted = 0;
    ih->decode_instruction = decode_instruction;
    ih->execute_instruction = execute_instruction;
    return ih;
}

void execute_format_i1(struct InstructionHandler *ih);
void execute_format_i2(struct InstructionHandler *ih);
void execute_format_i3(struct InstructionHandler *ih, struct ProgramCounter* pc);
void execute_format_i4(struct InstructionHandler *ih);
void execute_format_r(struct InstructionHandler *ih);

void execute_format_u1(struct InstructionHandler *ih);
void execute_format_u2(struct InstructionHandler *ih);


instruction_err_t decode_instruction(struct InstructionHandler *ih, int bits, struct ProgramCounter* pc)
{

    switch (bits & 0b1111111)
    {
    case 0b0110011: // function type R
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rs2 = (bits >> 20) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->funct7 = (bits >> 25) & 0b1111111;
        execute_format_r(ih);
        break;

    case 0b0010011: // function type I-1 (Immediate Operations)
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (bits >> 20) & 0b111111111111;
        execute_format_i1(ih);
        break;

    case 0b0000011: // function type I-2 (Load Store)
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (bits >> 20) & 0b111111111111;
        execute_format_i2(ih);
        break;

    case 0b1100111: // function type I-3 (Jump and Link reg)
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (bits >> 20) & 0b111111111111;
        execute_format_i3(ih, pc);
        break;

    case 0b1110011: // function type I-4 (Enviroment Calls)
        ih->rs1 = (bits >> 15) & 0b11111;
        ih->rd = (bits >> 7) & 0b11111;
        ih->funct3 = (bits >> 12) & 0b111;
        ih->imm = (bits >> 20) & 0b111111111111;
        execute_format_i4(ih);
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

    case 0b0110111: // function type U (Load Upper Imm)
        ih->rd = (bits >> 7) & 0b11111;
        ih->imm = (bits & (0b11111111111111111111)) << 12;
        execute_format_u1(ih);
        break;
    case 0b0010111: // function type U (Add Upper Imm To PC)
        ih->rd = (bits >> 7) & 0b11111;
        ih->imm = (bits & (0b11111111111111111111)) << 12;
        execute_format_u2(ih);
        break;

    default:
        printf("Function Type Not Existing: %d\n", (bits & 0b1111111));
        return UNKNOWN;
    }

    ih->registers[0] = 0; // x0 is hardwired to zero
    return OK;
}

void execute_format_r(struct InstructionHandler *ih)
{
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
    case 0x4:
    {
        // XOR
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];

        ih->registers[ih->rd] = src_1 ^ src_2;
        printf("XOR x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x6:
    {
        // OR
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];

        ih->registers[ih->rd] = src_1 | src_2;
        printf("OR x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x7:
    {
        // OR
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];

        ih->registers[ih->rd] = src_1 & src_2;
        printf("AND x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x1:
    {
        // SLL
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];

        ih->registers[ih->rd] = src_1 << src_2;
        printf("SLL x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x5:
    {
        // SRL
        if (ih->funct7 == 0)
        {
            int src_1 = ih->registers[ih->rs1];
            int src_2 = ih->registers[ih->rs2];

            ih->registers[ih->rd] = src_1 >> src_2;
            printf("SRL x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        }
        // SRA
        if (ih->funct7 == 0x20)
        {
            // TODO (NOT WORKING!!!)
            int src_1 = ih->registers[ih->rs1];
            int src_2 = ih->registers[ih->rs2];

            ih->registers[ih->rd] = src_1 >> src_2;
            printf("SRA x%d(%d), x%d(%d) -> x%d(%d)\n -- NOT IMPLEMENTED CORRECTLY", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        }
        break;
    }

    case 0x2:
    {
        // SLT
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];
        ih->registers[ih->rd] = (src_1 < src_2 ? 1 : 0);
        printf("SLT x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x3:
    {
        // SLTU TODO NOT WORKING!!
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->registers[ih->rs2];
        ih->registers[ih->rd] = (src_1 < src_2 ? 1 : 0);
        printf("SLTU x%d(%d), x%d(%d) -> x%d(%d) -- NOT IMPLEMENTED CORRECTLY\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }
    default:
        printf("UNKOWN FUNC: %d\n", ih->funct3);
        break;
    }
}

void execute_format_i1(struct InstructionHandler *ih)
{
    switch (ih->funct3)
    {
    case 0:
    { // ADDI
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src + ih->imm;
        printf("ADDI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;
    }
    case 0x4:
    {
        // XORI
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src ^ ih->imm;
        printf("XORI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x6:
    {
        // OR
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src | ih->imm;
        printf("ORI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x7:
    {
        // ANDI
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src & ih->imm;
        printf("ANDI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x1:
    {
        // SLLI
        int src = ih->registers[ih->rs1];
        ih->registers[ih->rd] = src << (ih->imm & 0b11111);
        printf("SLLI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x5:
    {
        // SRLI
        if (((ih->imm >> 5) & 0b11111111) == 0x00)
        {
            // SLLI
            int src = ih->registers[ih->rs1];
            ih->registers[ih->rd] = src >> (ih->imm & 0b11111);
            printf("SRLI x%d(%d) + %d -> x%d(%d)\n", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
            break;
        }
        // SRAI
        if (((ih->imm >> 5) & 0b11111111) == 0x20)
        {
            // TODO (NOT WORKING!!!)
            int src = ih->registers[ih->rs1];
            ih->registers[ih->rd] = src >> (ih->imm & 0b11111);
            printf("SRAI x%d(%d) + %d -> x%d(%d)\n -- NOT IMPLEMENTED CORRECTLY", ih->rs1, src, ih->imm, ih->rd, ih->registers[ih->rd]);
        }
        break;

    }

    case 0x2:
    {
        // SLTI
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;
        ih->registers[ih->rd] = (src_1 < src_2 ? 1 : 0);
        printf("SLTI x%d(%d), x%d(%d) -> x%d(%d)\n", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x3:
    {
        // SLTIU TODO NOT WORKING!!
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;
        ih->registers[ih->rd] = (src_1 < src_2 ? 1 : 0);
        printf("SLTU x%d(%d), x%d(%d) -> x%d(%d)\n -- NOT IMPLEMENTED CORRECTLY", ih->rs1, src_1, ih->rs2, src_2, ih->rd, ih->registers[ih->rd]);
        break;
    }
    default:
        printf("UNKOWN FUNC: %d\n", ih->funct3);
        break;
    }
}

void execute_format_i2(struct InstructionHandler *ih)
{
    switch (ih->funct3)
    {
    case 0:
    { // LB
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;

        ih->registers[ih->rd] = (ih->memory[src_1 + src_2] & 0xFF);

        printf("LB M[%d] -> x%d(%d)\n", (src_1 + src_2), ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x1:
    { // LH
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;

        ih->registers[ih->rd] = (ih->memory[src_1 + src_2] & 0xFFFF);

        printf("LH M[%d] -> x%d(%d)\n", (src_1 + src_2), ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x2:
    { // LW
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;

        ih->registers[ih->rd] = ih->memory[src_1 + src_2];

        printf("LW M[%d] -> x%d(%d)\n", (src_1 + src_2), ih->rd, ih->registers[ih->rd]);
        break;
    }

    case 0x4:
    {
        // LBU
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;

        ih->registers[ih->rd] = (ih->memory[src_1 + src_2] & 0xFF);

        printf("LBU M[%d] -> x%d(%d)\n", (src_1 + src_2), ih->rd, ih->registers[ih->rd]);
     
        break;
    }

    case 0x5:
    {
        // LHU
        int src_1 = ih->registers[ih->rs1];
        int src_2 = ih->imm;

        ih->registers[ih->rd] = (ih->memory[src_1 + src_2] & 0x0FFFF);

        printf("LHU M[%d] -> x%d(%d)\n", (src_1 + src_2), ih->rd, ih->registers[ih->rd]);
        break;
    }

    default:
        printf("UNKOWN FUNC: %d\n", ih->funct3);
        break;
    }
}

void execute_format_i3(struct InstructionHandler *ih, struct ProgramCounter* pc)
{
    int src = pc->cnt;
    int src1 = ih->rs1;
    int src2 = ih->imm;
    ih->registers[ih->rd] = src + 4;

    pc->cnt = src1 + src2;
    printf("JALR\n");
}

void execute_format_i4(struct InstructionHandler *ih)
{
    printf("E");
    if ((ih->imm & 0b1111) == 0x0) {
        printf("CALL\n");
        if (ih->registers[17] == 10) {   
            ih->halted = 1;
        }
        sleep(0);
    } else if ((ih->imm & 0b1111) == 0x1) {
        printf("BREAK\n");
    }

}

void execute_format_u1(struct InstructionHandler *ih) {
    int src = (ih->imm) << 12;
    ih->registers[ih->rd] = src;
    printf("imm(%d) << 12 -> x%d(%d)\n", ih->imm, ih->rd, ih->registers[ih->rd]);
}


void execute_format_u2(struct InstructionHandler *ih) {

}


instruction_err_t execute_instruction(struct InstructionHandler *ih, instruction_t instruction)
{

    return 0;
}