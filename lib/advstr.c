/*
 *  Copyright (c) 2014	Lee <TofuleeSoil@163.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <linux/string.h>
#include <debug.h>


char *strdel_tail(const char *str, char buf[],int chr, int n)
{
   //DEBUG_CHECK(!str && !buf && !n, NULL);
   //int len = strlen(str);
   char *dest;

   dest = strnchr(str,chr,n);
   int pos = (int)(dest - str);
   int count;
   for (count = 0; count < pos; count++ )
        buf[count] = str[count];
   buf[count - 1] = '\0';
   
   return buf;
}

char * strdel_head(const char *str, char buf[],int chr, int n)
{
   //DEBUG_CHECK(!str && !buf && !n, NULL);
   int len = strlen(str);
   int count;
   char *dest;

   dest = strnchr(str,chr,n);
   int pos = (int)(dest - str);
   for (count = pos; count < len; count++)
        buf[count - pos] = str[count];
   buf[count] = '\0';

   return buf;
}

char *str_split(const char *src,char buf[],int chr, int n1, int n2)
{   
    //DEBUG_CHECK(!src && !buf && !n1 && !n2, NULL);
    char tmp[50];
    if (!strdel_head(src,tmp,chr,n1))
       return NULL;
    if (!strdel_tail(tmp,buf,chr,n2 - n1))
       return NULL;

    return buf;
}

