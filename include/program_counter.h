#include <stdio.h>

typedef struct ProgramCounter
{
    program_cnt_err_t (*init)(struct program_counter *self);
    program_cnt_err_t (*increment_counter)(struct ProgramCounter *self);
} program_cnt_t;

typedef enum ProgramCounterErr 
{
    IVALID_CNT,
} program_cnt_err_t;