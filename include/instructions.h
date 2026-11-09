
typedef enum Instruction {
    ADD_INS,
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
} instruction_err_t;

/// @brief Takes an instruction of type: `instruction_t`, and executes it.
/// @param self
/// @param instruction
/// @return `instruction_err_t`
instruction_err_t execute_instruction(struct instruction_handler_t *self, instruction_t instruction);

/// @brief Takes the inputs bits, and converts it into the `Instruction` type.
/// @param self 
/// @return `instruction_err_t
instruction_err_t decode_instruction(struct instruction_handler_t *self);

/// @brief Initialize self.
/// @param self 
/// @return `instruction_err_t
instruction_err_t init(struct instruction_handler_t *self);

typedef struct InstructionHandler {

    instruction_err_t (*init)(struct InstructionHandler *self);
    instruction_err_t (*decode_instruction)(struct InstructionHandler *self); // also something that resembles the instruction in bits
    instruction_err_t (*execute_instruction)(struct InstructionHandler *self, Instruction instruction);
} instruction_handler_t;