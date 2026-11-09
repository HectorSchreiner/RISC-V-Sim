#include <stdio.h>
#include "file_handler.h"
#include "instructions.h"
#include "program_counter.h"

int main(void)
{
   struct FileHandler* fh = create_filehandler();
   struct ProgramCounter* pc = create_counter();
   struct InstructionHandler* ih = create_instruction_handler();

   int* instructions_b = fh->read_file(fh);
   
   pc->cnt = 0;
   pc->increment = 4;

   for (int i = 0; i < 10; i++) {
      printf("PC: %d\n", pc->cnt);
      ih->decode_instruction(ih, instructions_b[pc->cnt / 4]);
      pc->increment_counter(pc);
   }

   return 0;
}
