#include <linux/string.h>
#include <debug.h>


int strdel_tail(char str[], char buf[],int chr, int n)
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

int strdel_head(char str[], char buf[],int chr, int n)
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
int str_split(char src[],char buf[],int chr, int n1, int n2)
{
    char tmp1[50];
    char tmp2[50];
    del_head(src,tmp1,chr,n1);
    del_tail(src,tmp2,chr,n2);
    strcommon(tmp1,tmp2,buf);
}

