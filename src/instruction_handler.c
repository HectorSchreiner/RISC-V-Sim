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
            ih->rs1    = bits>>15 & 0b11111;
            ih->rs2    = bits>>20 & 0b11111;
            ih->rd     = bits>>7  & 0b11111;
            ih->funct3 = bits>>12 & 0b111;
            ih->funct7 = bits>>25 & 0b1111111;
            printf("function type R\n");
            break;
        
        case 0b0010011: // function type I
            ih->rs1    = bits>>15 & 0b11111;
            ih->rd     = bits>>7  & 0b11111;
            ih->funct3 = bits>>12 & 0b111;
            ih->imm    = bits>>20 & 0b111111111111;
            printf("function type I\n");
            break;
        
        default:
            printf("stupid ass\n");
            return UNKNOWN;        
    }
}

instruction_err_t execute_instruction(struct InstructionHandler* ih, instruction_t instruction) {
    return 0;
}