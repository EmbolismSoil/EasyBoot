/*
 *  S3C2440 NANDFLASH DRIVER
 *
 *  Copyright (c) 2014  Lee <TofuleeSoil@163.com>
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


#include <config.h>
#include <debug.h>
#include <driver.h>
#include <printf.h>
#include <asm/arch-s3c24x0/s3c2440.h>

//#define RUN_STAND

#ifdef RUN_STAND
#define NFCONF (*((volatile unsigned long *)0x4E000000))
#define NFCONT (*((volatile unsigned long *)0x4E000004))
#define NFCMMD (*((volatile unsigned char *)0x4E000008))
#define NFADDR (*((volatile unsigned char *)0x4E00000C))
#define NFDATA (*((volatile unsigned char *)0x4E000010))
#define NFSTAT (*((volatile unsigned char *)0x4E000020))
#endif

int nand_open(void* unuse0, void *unuse1)
{
#define TACLS   0
#define TWRPH0  1
#define TWRPH1  0
	
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	nand_ptr->nfconf = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	nand_ptr->nfcont = (1<<4) | (1<<1) | (1<<0);
#else
	/* 设置时序 */
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	/* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
	NFCONT = (1<<4)|(1<<1)|(1<<0);	
#endif
}

static void nand_select(void)
{
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	nand_ptr->nfcont &= ~(1<<1);
#else
	NFCONT &= ~(1<<1);	
#endif
}

static void nand_deselect(void)
{
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();	
	nand_ptr->nfcont |= (1<<1);
#else
	NFCONT |= (1<<1);
#endif
}

static void nand_cmd(unsigned char cmd)
{
	volatile int i;
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	volatile unsigned char *p = (volatile unsigned char *)&nand_ptr->nfcmd;
	*p = cmd;
#else
	NFCMMD = cmd;
#endif
	for (i = 0; i < 10; i++);
}

static void nand_addr(unsigned int addr)
{
	unsigned int col  = addr % 2048;
	unsigned int page = addr / 2048;
	volatile int i;
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	volatile unsigned char *p = (volatile unsigned char *)&nand_ptr->nfaddr;
		
	   *p = col & 0xff;
        for (i = 0; i < 10; i++);
        *p = (col >> 8) & 0xff;
        for (i = 0; i < 10; i++);

        *p  = page & 0xff;
        for (i = 0; i < 10; i++);
        *p  = (page >> 8) & 0xff;
        for (i = 0; i < 10; i++);
       *p = (page >> 16) & 0xff;
        for (i = 0; i < 10; i++);
#else

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
#endif
}

static void nand_wait_ready(void)
{
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	volatile unsigned char *p = (volatile unsigned char *)&nand_ptr->nfstat;
	while (!( (*p) & 1));
	
#else
	while (!(NFSTAT & 1));
#endif	
}

static unsigned char nand_data(void)
{
#ifndef RUN_STAND
	struct s3c2440_nand *const nand_ptr = s3c2440_get_base_nand();
	volatile unsigned char *p = (volatile unsigned char *)&nand_ptr->nfdata;
	return *p;
#else	
	return NFDATA;
#endif	
}

int nand_read(unsigned int addr, void *argbuf, unsigned int len)
{
        unsigned char *buf = (unsigned char *)argbuf;
	int col = addr % 2048;
	int i = 0;
	unsigned int count = 0;
	int flag = 1;
	int strlen = 100;


	int nand_boot;
	if (addr == 0)
	   nand_boot = 1;
	else 
	   nand_boot = 0;

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
		
		if (!nand_boot)		
		if (addr > count*(len/100) && addr <= len){
		    PUT_STR("\b\b\b\b\b\b\b\b\b\b\b\b");
                    PUT_STR("| ---- ");
		    PUT_DEC(count);
		    PUT_STR("%");
		    if (count >= 30 * flag){
			PUT_STR("\n\r");
			flag++;
		    }
		    count ++;
		}
		col = 0;
	}

	if(!nand_boot){
	PUT_STR("\b\b\b\b\b\b\b\b\b\b\b\b");
        PUT_STR("| ---- ");
	PUT_DEC(100);
	PUT_STR("%");
	PUT_STR("\n\rOK!");
	}
	/* 7. 取消选中 */		
	nand_deselect();
}

d_ops nand_ops = {.open = nand_open,.read = nand_read};
DRIVER_EXPORT(NAND, NULL, &nand_ops, ((void *)0) );
