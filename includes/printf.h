#ifndef __PRINTF_H__
#define __PRINTF_H__

#define CFG_PBSIZE 1024
#include <stdarg.h>


extern void printf (const char *fmt, ...);

/* lib_generic/vsprintf.c */
extern unsigned long   simple_strtoul(const char *cp,char **endp,unsigned int base);
#ifdef CFG_64BIT_VSPRINTF
extern unsigned long long      simple_strtoull(const char *cp,char **endp,unsigned int base);
#endif
extern long    simple_strtol(const char *cp,char **endp,unsigned int base);
extern void    panic(const char *fmt, ...);
extern int     sprintf(char * buf, const char *fmt, ...);
extern int     vsprintf(char *buf, const char *fmt, va_list args);


#endif
