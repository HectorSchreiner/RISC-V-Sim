#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "instructions.h"
#include "program_counter.h"

static void dump_registers(struct InstructionHandler *ih) {
   printf("\nREGISTERS:\n");
   for (int i = 0; i < 32; i++) {
      if (ih->registers[i] != 0) {
         printf("\tx%-2d = 0x%08x (%d)\n", i, ih->registers[i], (int32_t)ih->registers[i]);
      }
   }
}

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

//   for (int i = 0; i < instruction_cnt; i++) {
//      printf("PC[+%d] - ", pc->cnt);
//     ih->decode_instruction(ih, instructions_b[pc->cnt], pc);
//      pc->increment_counter(pc);
//   }

   dump_registers(ih);

   free(ih);
   free(pc);
   free(fh);

   return 0;
}
