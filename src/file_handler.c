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

uint8_t* read_file(struct FileHandler *fh, char *filename, int *size)
{
   //char filename[] = "/home/anders/Documents/git-repos/RISC-V-Sim/tests/addlarge.bin";
   (void)fh;
   char filename[] = "tests/addlarge.bin";


   FILE *fptr = fopen(filename, "rb");
   if ((fptr ) == NULL) {
      printf("Error! opening file");
      return NULL;
   }
   
   if (fseek(fptr, 0, SEEK_END) != 0) {
      fclose(fptr);
      return NULL;
   }

   long len = ftell(fptr);
   if (len < 0) {
      fclose(fptr);
      return NULL;
   }

   rewind(fptr);

   uint8_t *buf = malloc(len > 0 ? (size_t)len : 1);

   if (!buf) {
      fclose(fptr);
      return NULL;
   }

   size_t n = fread(buf, 1, (size_t)len, fptr);
   fclose(fptr);

   if (n != (size_t)len) {
      free(buf);
      return NULL;
   }

   *size = n;

   return buf;
}