#include "program_counter.h"
#include <stdlib.h>

struct ProgramCounter* create_counter() {
   struct ProgramCounter* pc = malloc((sizeof(struct ProgramCounter)));
   pc->cnt = 0;
   pc->increment = 4;
   pc->increment_counter = increment_counter;
   pc->set_counter = set_counter;
   return pc;
}

program_cnt_err_t increment_counter(ProgramCounter* pc) {
   pc->cnt += pc->increment;
   return OK;
}

program_cnt_err_t set_counter(ProgramCounter* pc, unsigned value) {
   pc->cnt = value;
   return OK;
}
