#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "program_counter.h"

#define MEM_SIZE (1u << 20)

typedef enum Instruction {
    // Upper immediate and jumps
    lui_i,
    auipc_i,
    jal_i,
    jalr_i,
    // Branches
    beq_i,
    bne_i,
    blt_i,
    bge_i,
    bltu_i,
    bgeu_i,
    // Load
    lb_i,
    lh_i,
    lw_i,
    lbu_i,
    lhu_i,
    // Store
    sb_i,
    sh_i,
    sw_i,
    // Immediate
    addi_i,
    xori_i,
    ori_i,
    andi_i,
    slli_i,
    srli_i,
    srai_i,
    slti_i,
    sltiu_i,
    // Operations
    add_i,
    sub_i,
    xor_i,
    or_i,
    and_i,
    sll_i,
    srl_i,
    sra_i,
    slt_i,
    sltu_i,
    // Enviroment Calls
    fence_i,
    ecall_i,
    ebreak_i,
    invalid_i
} instruction_t;

typedef enum Register {
    t0,
    t1,
    t2,
    x0,
    x1,
    x2,
    x3
    // ...
} reg_t;

typedef enum InstructionErr {
    OK,
    UNKNOWN_INSTR,
    MEM_ERR
} instruction_err_t;

struct InstructionHandler {
    uint32_t registers[32];
    uint8_t memory[MEM_SIZE];
    int rs1;
    int rs2;
    int rd;
    int funct7;
    int funct3;
    int32_t imm;
    int halted;

    instruction_err_t (*decode_instruction)(struct InstructionHandler *self, int bits, struct ProgramCounter* pc); // also something that resembles the instruction in bits
    instruction_err_t (*execute_instruction)(struct InstructionHandler *self, instruction_t instruction, struct ProgramCounter* pc);
};

struct InstructionHandler* create_instruction_handler();

/// @brief Takes an instruction of type: `instruction_t`, and executes it.
/// @param self
/// @param instruction
/// @return `instruction_err_t`
instruction_err_t execute_instruction(struct InstructionHandler *self, instruction_t instruction, struct ProgramCounter* pc);

/// @brief Takes the inputs bits, and converts it into the `Instruction` type.
/// @param self 
/// @return `instruction_err_t
instruction_err_t decode_instruction(struct InstructionHandler *self, int bits, struct ProgramCounter* pc);

#endif
