#include "program_counter.h"

program_cnt_err_t init_counter(program_cnt_t* pc, unsigned init_cnt, unsigned increment) {
   if (increment) { // error handling for now...
      pc->cnt = init_cnt;
      pc->increment = increment;
      return OK;
   } else {
      return CREATION_ERR;
   }
}

program_cnt_err_t increment_counter(program_cnt_t* pc) {
   pc->cnt += pc->increment;
   return OK;
}

program_cnt_err_t set_value(program_cnt_t* pc, unsigned value) {
   pc->cnt = value;
   return OK;
}
