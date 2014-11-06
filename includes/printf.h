#ifndef __PRINTF_H__
#define __PRINTF_H__

#define CFG_PBSIZE 1024
extern void printf (const char *fmt, ...);

/* lib_generic/vsprintf.c */
unsigned long   simple_strtoul(const char *cp,char **endp,unsigned int base);
#ifdef CFG_64BIT_VSPRINTF
unsigned long long      simple_strtoull(const char *cp,char **endp,unsigned int base);
#endif
long    simple_strtol(const char *cp,char **endp,unsigned int base);
void    panic(const char *fmt, ...);
int     sprintf(char * buf, const char *fmt, ...);
int     vsprintf(char *buf, const char *fmt, va_list args);


#endif
