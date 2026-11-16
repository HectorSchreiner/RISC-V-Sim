#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

struct InstructionHandler* create_instruction_handler() {
    struct InstructionHandler* ih = malloc((sizeof(struct InstructionHandler)));
    ih->decode_instruction = decode_instruction;
    ih->execute_instruction = execute_instruction;
    return ih;
}


instruction_err_t decode_instruction(struct InstructionHandler* ih, int bits) {
    printf("here");
    switch (bits & 0b1111111)
    {
        case 0b0110011: // function type R
            ih->rs1    = (bits>>15) & 0b11111;
            ih->rs2    = (bits>>20) & 0b11111;
            ih->rd     = (bits>>7)  & 0b11111;
            ih->funct3 = (bits>>12) & 0b111;
            ih->funct7 = (bits>>25) & 0b1111111;
            printf("function type R\n");
            break;
        
        case 0b0010011: // function type I
        case 0b1100111:
        case 0b1110011:
        case 0b0000011:
            ih->rs1    = (bits>>15) & 0b11111;
            ih->rd     = (bits>>7)  & 0b11111;
            ih->funct3 = (bits>>12) & 0b111;
            ih->imm    = (bits>>20) & 0b111111111111;
            printf("function type I\n");
            break;

        case 0b0100011: // function type S
            ih->rs1    = (bits>>15) & 0b11111;
            ih->rs2    = (bits>>20) & 0b11111;
            // ih->rd     = bits>>7  & 0b11111;
            ih->funct3 = (bits>>12) & 0b111;
            ih->imm    = (((bits>>25) & 0b1111111) << 5) | ((bits >> 7) & 0b11111);
            printf("function type S\n");
            break;

        case 0b1100011: // function type B
            ih->rs1    = bits>>15 & 0b11111;
            ih->rs2    = bits>>20 & 0b11111;
            // ih->rd     = bits>>7  & 0b11111;
            ih->funct3 = bits>>12 & 0b111;
            ih->imm    = (((bits>>31) & 0b1) << 12) | (((bits>>7) & 0b1) << 11) | (((bits>>8) & 0b1111) << 1) | (((bits>>25) & 0b111111) << 5);
            printf("function type B\n");
            break;
        
        case 0b1101111: // function type J
            // ih->rs1    = bits>>15 & 0b11111;
            // ih->rs2    = bits>>20 & 0b11111;
            ih->rd     = (bits>>7)  & 0b11111;
            // ih->funct3 = bits>>12 & 0b111;
            ih->imm    = (((bits>>31) & 0b1)<<20) | (((bits>>21) & 0b1111111111)<<9) | (((bits>>20) & 0b1)<<8) | ((bits>>12) & 0b11111111);
            printf("function type J\n");
            break;

        case 0b0110111: // function type U
        case 0b0010111:
            // ih->rs1    = bits>>15 & 0b11111;
            // ih->rs2    = bits>>20 & 0b11111;
            ih->rd     = (bits>>7)  & 0b11111;
            // ih->funct3 = bits>>12 & 0b111;
            ih->imm    = bits & (0b11111111111111111111) << 12;
            printf("function type U\n");
            break;
        
        default:
            printf("stupid ass\n");
            return UNKNOWN;        
    }
}

instruction_err_t execute_instruction(struct InstructionHandler* ih, instruction_t instruction) {
    return 0;
}
