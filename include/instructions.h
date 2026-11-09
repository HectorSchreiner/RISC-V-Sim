
typedef enum Instruction {
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
    addi_i,
    xori_i,
    ori_i,
    andi_i,
    slli_i,
    srli_i,
    srai_i,
    slti_i,
    sltiu_i,
    lb_i,
    lh_i,
    lw_i,
    lbu_i,
    lhu_i,
    sb_i,
    sh_i,
    sw_i,
    beq_i,
    bne_i,
    blt_i,
    bge_i,
    bltu_i,
    bgeu_i,
    jal_i,
    jalr_i,
    lui_i,
    auipc_i,
    ecall_i,
    ebreak_i
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
} register_t;

typedef enum InstructionErr {
    UNKNOWN,
    OK,
} instruction_err_t;

typedef struct InstructionHandler {
    int rs1;
    int rs2;
    int rd;
    int funct7;
    int funct3;
    int imm;
    instruction_err_t (*decode_instruction)(struct InstructionHandler *self, int bits); // also something that resembles the instruction in bits
    instruction_err_t (*execute_instruction)(struct InstructionHandler *self, instruction_t instruction);
} instruction_handler_t;

struct InstructionHandler* create_instruction_handler();

/// @brief Takes an instruction of type: `instruction_t`, and executes it.
/// @param self
/// @param instruction
/// @return `instruction_err_t`
instruction_err_t execute_instruction(struct InstructionHandler *self, instruction_t instruction);

/// @brief Takes the inputs bits, and converts it into the `Instruction` type.
/// @param self 
/// @return `instruction_err_t
instruction_err_t decode_instruction(struct InstructionHandler *self, int bits);