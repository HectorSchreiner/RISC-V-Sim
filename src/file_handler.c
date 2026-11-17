#include "file_handler.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>

struct FileHandler* create_filehandler() {
   struct FileHandler* fh = malloc((sizeof(struct FileHandler)));
   fh->read_file = read_file;
   return fh;
}

int* read_file(struct FileHandler *fh, int *size)
{
   char filename[] = "/home/anders/Documents/git-repos/RISC-V-Sim/tests/addlarge.bin";

   FILE *fptr = fopen(filename, "rb");
   if ((fptr ) == NULL) {
      printf("Error! opening file");
      return NULL;
   }
   
   __uint32_t value;
   struct stat st;
   stat(filename, &st);
   *size = st.st_size;

   int *instructions_b = malloc((size_t)*size);
   int cnt = 0;
   
   while (fread(&value, sizeof(value), 1, fptr) == 1) {
      instructions_b[cnt] = value;
      cnt+=4;
   }

   fclose(fptr);

   return instructions_b;
}