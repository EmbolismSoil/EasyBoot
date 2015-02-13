#include <driver.h>
#define GPBCON      (*(volatile unsigned long *)0x56000010)
#define GPBDAT      (*(volatile unsigned long *)0x56000014)

/*
 * LED1,LED2,LED4¶ÔÓ¦GPB5¡¢GPB6¡¢GPB7¡¢GPB8
 */
#define GPB5_out        (1<<(5*2))
#define GPB6_out        (1<<(6*2))
#define GPB7_out        (1<<(7*2))
#define GPB8_out        (1<<(8*2))


/* NAND FLASH¿ØÖÆÆ÷ */
#define NFCONF (*((volatile unsigned long *)0x4E000000))
#define NFCONT (*((volatile unsigned long *)0x4E000004))
#define NFCMMD (*((volatile unsigned char *)0x4E000008))
#define NFADDR (*((volatile unsigned char *)0x4E00000C))
#define NFDATA (*((volatile unsigned char *)0x4E000010))
#define NFSTAT (*((volatile unsigned char *)0x4E000020))

/* GPIO */
#define GPHCON              (*(volatile unsigned long *)0x56000070)
#define GPHUP               (*(volatile unsigned long *)0x56000078)

/* UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)

#define TXD0READY   (1<<2)


#define PCLK            50000000    // init.cÖÐµÄclock_initº¯ÊýÉèÖÃPCLKÎª50MHz
#define UART_CLK        PCLK        //  UART0µÄÊ±ÖÓÔ´ÉèÎªPCLK
#define UART_BAUD_RATE  115200      // ²¨ÌØÂÊ
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

#if 0
static void  wait(volatile unsigned long dly)
{
        for(; dly > 0; dly--);
}

#endif


int uart_open(void *arg1, void *arg2)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3ÓÃ×÷TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3ÄÚ²¿ÉÏÀ­

    ULCON0  = 0x03;     // 8N1(8¸öÊý¾ÝÎ»£¬ÎÞ½ÏÑé£¬1¸öÍ£Ö¹Î»)
    UCON0   = 0x05;     // ²éÑ¯·½Ê½£¬UARTÊ±ÖÓÔ´ÎªPCLK
    UFCON0  = 0x00;     // ²»Ê¹ÓÃFIFO
    UMCON0  = 0x00;     // ²»Ê¹ÓÃÁ÷¿Ø
    UBRDIV0 = UART_BRD; // ²¨ÌØÂÊÎª115200

    return 0;
}

/*
 * ·¢ËÍÒ»¸ö×Ö·û
 */
static void putc(unsigned char c)
{
    /* µÈ´ý£¬Ö±µ½·¢ËÍ»º³åÇøÖÐµÄÊý¾ÝÒÑ¾­È«²¿·¢ËÍ³öÈ¥ */
    while (!(UTRSTAT0 & TXD0READY));

    /* ÏòUTXH0¼Ä´æÆ÷ÖÐÐ´ÈëÊý¾Ý£¬UART¼´×Ô¶¯½«Ëü·¢ËÍ³öÈ¥ */
    UTXH0 = c;
}


int uart_write(unsigned int addr ,void *buf, unsigned int len)
{
        int i = 0;
	char *str = (char *)buf;
        while (str[i])
        {
                putc(str[i]);
                i++;
        }
        
    return 0;                                                                
}

void puthex(unsigned int val)
{
        /* 0x1234abcd */
        int i;
        int j;

        uart_write(0,"0x",0);

        for (i = 0; i < 8; i++)
        {
                j = (val >> ((7-i)*4)) & 0xf;
                if ((j >= 0) && (j <= 9))
                        putc('0' + j);
                else
                        putc('A' + j - 0xa);

        }

}

void putdec(int val)
{
     int dec1 = val%10;
     int dec2 = val%100/10;
     int dec3 = val%1000/100;
     int dec4 = val%10000/1000;
     int dec5 = val/10000;
     putc('0' + dec5);
     putc('0' + dec4);
     putc('0' + dec3);
     putc('0' + dec2);
     putc('0' + dec1);
}

d_ops uart_ops = {.open = uart_open,.write = uart_write};

DRIVER_EXPORT(UART, NULL, &uart_ops, ((void *)0) );

