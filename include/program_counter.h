#include <stdio.h>

typedef enum ProgramCounterErr
{
    IVALID_CNT,
    CREATION_ERR
} program_cnt_err_t;

/// @brief Handles incrementing the program counter.
/// Use program_counter->increment_counter() to increment the counter.
typedef struct ProgramCounter
{
    int cnt, increment;
    program_cnt_err_t (*increment_counter)(struct ProgramCounter *self);
    program_cnt_err_t (*set_counter)(struct ProgramCounter *self, unsigned value);
} ProgramCounter;

struct ProgramCounter* create_counter();

program_cnt_err_t increment_counter(ProgramCounter* pc);

program_cnt_err_t set_counter(ProgramCounter* pc, unsigned value);
