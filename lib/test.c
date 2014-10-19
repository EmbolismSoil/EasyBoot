#include "string.h"
#include <stdio.h>
void main()
{
   char *dest; 
   dest = strnchr("/smdk2410/class/device",'/',2);
   printf("%s:\n",dest);
}
