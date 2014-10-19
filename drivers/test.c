#include <string.h>

char * del_tail(char str[], char buf[],int chr, int n)
{
   int len = strlen(str);
   char *dest;

   dest = strnchr(str,chr,n);
   int pos = (int)(dest - str);
   int count;
   for (count = 0; count < pos; count++ )
	buf[count] = str[count];
   buf[count - 1] = '\0';
}

char *del_head(char str[], char buf[],int chr, int n)
{
   int len = strlen(str);
   int count;
   char *dest;

   dest = strnchr(str,chr,n);
   int pos = (int)(dest - str); 
   for (count = pos; count < len; count++)
        buf[count - pos] = str[count];
   buf[count] = '\0';
}

char *get_path(char src[],char buf[],int chr, int n)
{
    char tmp[100];
    del_head(src,tmp,chr,n);
    del_tail(tmp,buf,chr,1);
}
void main()
{
  char  buf[100];
  get_path("/smdk2410/class/device",buf,'/',2);
  printf("%s\n",buf);
}
