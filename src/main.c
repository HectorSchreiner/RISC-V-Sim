#include <stdio.h>
#include "file_handler.h"
#include "instructions.h"
#include "program_counter.h"

int main(void)
{
   int instruction_cnt = 0;

   struct FileHandler* fh = create_filehandler();
   struct ProgramCounter* pc = create_counter();
   struct InstructionHandler* ih = create_instruction_handler();

   int* instructions_b = fh->read_file(fh, &instruction_cnt);
   
   pc->cnt = 0;
   pc->increment = 4;

   instruction_cnt /= 4;
   printf("the program is %d instructions\n", instruction_cnt);

 while (!ih->halted && pc->cnt / 4 < instruction_cnt) {
      printf("PC[+%d] - ", pc->cnt);
      ih->decode_instruction(ih, instructions_b[pc->cnt / 4], pc);
      pc->increment_counter(pc);
   }

   return 0;
}
