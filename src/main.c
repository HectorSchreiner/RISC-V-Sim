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
   struct FileHandler* fh = create_filehandler();
   struct ProgramCounter* pc = create_counter();
   struct InstructionHandler* ih = create_instruction_handler();
   
   int instruction_cnt = 0;
   uint8_t* instructions_b = fh->read_file(fh, "/home/anders/Documents/git-repos/RISC-V-Sim/tests/addlarge.bin", &instruction_cnt);
   
   if (!instructions_b) {
      return 1;
   }

   memcpy(ih->memory, instructions_b, instruction_cnt);
   free(instructions_b);

   printf("Loaded %d bytes (%d instructions)\n", instruction_cnt, instruction_cnt / 4);

   pc->cnt = 0;
 
   while (!ih->halted) {
      uint32_t addr = (uint32_t)pc->cnt;
      if (addr > MEM_SIZE - 4) {
         fprintf(stderr, "Bad PC 0x%08x\n", addr);
         break;
      }

      uint32_t bits = ((uint32_t)ih->memory[addr]) 
                  | ((uint32_t)ih->memory[addr + 1] << 8)
                  | ((uint32_t)ih->memory[addr + 2] << 16)
                  | ((uint32_t)ih->memory[addr + 3] << 24);
      
      printf("PC=+%03d  instr=0x%08x\n", addr, bits);

      if (ih->decode_instruction(ih, bits, pc) != OK) { break; }
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
