#include "file_handler.h"
#include <stdio.h>

file_err_t read_file(char filename[])
{
   FILE *fp = fopen(filename, "r");
   char ch;

   if (fp == NULL) {
      printf("Unable to open file.");
      return FILENAME_DOESNT_EXIST_ERR;
   }

   return UNKNOWN_ERR;
}


