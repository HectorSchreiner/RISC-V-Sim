#include <stdio.h>
#include "file_handler.h"
#include "instructions.h"
#include "program_counter.h"

int main(void)
{
   int instruction_bytes = 0;

   struct FileHandler* fh = create_filehandler();
   struct ProgramCounter* pc = create_counter();
   struct InstructionHandler* ih = create_instruction_handler();

   int* instructions_b = fh->read_file(fh, &instruction_bytes);
   

   pc->cnt = 0;
   pc->increment = 1;


   printf("the program is %d instructions\n", instruction_bytes / 4);

   while(pc->cnt < instruction_bytes / 4){
      printf("PC[+%d] - ", pc->cnt);
      ih->decode_instruction(ih, instructions_b[pc->cnt]);
      pc->increment_counter(pc);
   }

   return 0;
}
