#ifndef __DEBUG_H
#define __DEBUG_H
extern int uart_write(unsigned int addr ,void *buf, unsigned int len);
extern void puthex(unsigned int val);
extern void putdec(int val);
#define PUT_STR(__str)  uart_write(0,__str,0)
#define PUT_HEX(__hex)  puthex(__hex)
#define PUT_DEC(__dec) putdec(__dec)

#define DEBUG_PUT()     PUT_STR("in function :");\
                        PUT_STR(__func__);\
                        PUT_STR("\n\rline:");\
                        PUT_DEC(__LINE__);\
			PUT_STR("\n\r")




#define DEBUG_CHECK(__condiction,ret)\
	if ((__condiction)){\
	 return (ret);}

#endif
