#include <stdio.h>
#include "file_handler.h"

int main(void)
{
   int r = read_file();
   printf("%d", r);
   return 0;
}
