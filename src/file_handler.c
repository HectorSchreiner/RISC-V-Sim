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
   char filename[] = "tests/addlarge.bin";

   FILE *fptr = fopen(filename, "rb");
   if (fptr == NULL) {
      printf("Error! opening file\n");
      return NULL;
   }

   struct stat st;
   if (stat(filename, &st) != 0) {
      fclose(fptr);
      return NULL;
   }
   *size = st.st_size;                       // size in BYTES

   int *instructions = malloc(st.st_size);
   if (!instructions) {
      fclose(fptr);
      return NULL;
   }

   fread(instructions, sizeof(int), st.st_size / sizeof(int), fptr);
   fclose(fptr);

   return instructions;
}