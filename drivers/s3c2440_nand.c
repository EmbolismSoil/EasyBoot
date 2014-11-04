#include <debug.h>
#include <driver.h>
/* NAND FLASH控制器 */
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


int nand_open(void* unuse0, void *unuse1)
{
#define TACLS   0
#define TWRPH0  1
#define TWRPH1  0
	/* 设置时序 */
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	/* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
	NFCONT = (1<<4)|(1<<1)|(1<<0);	
}

static void nand_select(void)
{
	NFCONT &= ~(1<<1);	
}

static void nand_deselect(void)
{
	NFCONT |= (1<<1);	
}

static void nand_cmd(unsigned char cmd)
{
	volatile int i;
	NFCMMD = cmd;
	for (i = 0; i < 10; i++);
}

static void nand_addr(unsigned int addr)
{
	unsigned int col  = addr % 2048;
	unsigned int page = addr / 2048;
	volatile int i;

	NFADDR = col & 0xff;
	for (i = 0; i < 10; i++);
	NFADDR = (col >> 8) & 0xff;
	for (i = 0; i < 10; i++);
	
	NFADDR  = page & 0xff;
	for (i = 0; i < 10; i++);
	NFADDR  = (page >> 8) & 0xff;
	for (i = 0; i < 10; i++);
	NFADDR  = (page >> 16) & 0xff;
	for (i = 0; i < 10; i++);	
}

static void nand_wait_ready(void)
{
	while (!(NFSTAT & 1));
}

static unsigned char nand_data(void)
{
	return NFDATA;
}

int nand_read(unsigned int addr, void *argbuf, unsigned int len)
{
        unsigned char *buf = (unsigned char *)argbuf;
	int col = addr % 2048;
	int i = 0;
	unsigned int count = 0;
	int flag = 1;	
 	PUT_STR("reading nand....\n\r");
	/* 1. 选中 */
	nand_select();

	while (i < len)
	{
		/* 2. 发出读命令00h */
		nand_cmd(0x00);

		/* 3. 发出地址(分5步发出) */
		nand_addr(addr);

		/* 4. 发出读命令30h */
		nand_cmd(0x30);

		/* 5. 判断状态 */
		nand_wait_ready();

		/* 6. 读数据 */
		for (; (col < 2048) && (i < len); col++)
		{
			buf[i] = nand_data();
			i++;
			addr++;
		}
		
		if (addr > count*(len/100) && addr <= len){
		    PUT_STR("\b\b\b\b\b\b\b\b\b\b\b\b");
                    PUT_STR("# ---- ");
		    PUT_DEC(count);
		    PUT_STR("%");
		    if (count > 30 * flag){
			PUT_STR("\n\r");
			flag++;
		    }
		    count ++;
		}
		col = 0;
	}
	PUT_STR("\b\b\b\b\b\b\b\b\b\b\b\b");
        PUT_STR("# ---- ");
	PUT_DEC(100);
	PUT_STR("%");
	PUT_STR("\n\rOK!");
	/* 7. 取消选中 */		
	nand_deselect();
}

d_ops nand_ops = {.open = nand_open,.read = nand_read};
DRIVER_EXPORT(NAND, NAND, NULL, &nand_ops, ((void *)0) );
