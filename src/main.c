#include <stdio.h>
#include "file_handler.h"
#include "program_counter.h"

int main(void)
{
   struct ProgramCounter* pc = create_counter();
   pc->cnt = 0;
   pc->increment = 4;
   printf("PC: %d\n", (int)pc->cnt);

   for (int i = 0; i < 10; i++) {
      pc->increment_counter(pc);
      printf("PC: %d\n", (int)pc->cnt);
   }

   return 0;
}
