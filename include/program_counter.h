#include <stdio.h>

typedef enum ProgramCounterErr
{
    OK,
    IVALID_CNT,
    CREATION_ERR
} program_cnt_err_t;

/// @brief Handles incrementing the program counter.
/// Use program_counter->increment_counter() to increment the counter.
typedef struct ProgramCounter
{
    int cnt, increment;
    program_cnt_err_t (*init_counter)(struct ProgramCounter *self, unsigned init_cnt, unsigned increment);
    program_cnt_err_t (*increment_counter)(struct ProgramCounter *self);
    program_cnt_err_t (*set_counter)(struct ProgramCounter *self, unsigned value);
} program_cnt_t;

program_cnt_err_t init_counter(program_cnt_t* pc, unsigned init_cnt, unsigned increment);

program_cnt_err_t increment_counter(program_cnt_t* pc);

program_cnt_err_t set_value(program_cnt_t* pc, unsigned value);
