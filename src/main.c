#include <stdio.h>
#include "file_handler.h"
#include "program_counter.h"

int main(void)
{
   program_cnt_t pc;
   init_counter(&pc, 174, 4);
   printf("PC: %d\n", (int)pc.cnt);

   for (int i = 0; i < 10; i++) {
      increment_counter(&pc);
      printf("PC: %d\n", (int)pc.cnt);
   }

   return 0;
}
