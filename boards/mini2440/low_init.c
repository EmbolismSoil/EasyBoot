/*
  * Mini2440  initialise
 *  Copyright (c) 2014 Lee <Tofulee@163.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#define MEM_REG_BASE  ((volatile unsigned long *)0x48000000)
#define CLK_REG_BASE  ((volatile unsigned long *)0x4c000000)
/*clock registers*/
#define LOCKTIME      0
#define MPLLCON       1
#define UPLLCON        2
#define CLKCON          3
#define CLKSLOW       4
#define CLKDIVN        5

#define MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))

 void setup_ram()
 {
	MEM_REG_BASE[0] = 0x22011110;    
	MEM_REG_BASE[1] = 0x00000700;     
	MEM_REG_BASE[2] = 0x00000700;    
	MEM_REG_BASE[3] = 0x00000700;    
	MEM_REG_BASE[4] = 0x00000700;   
	MEM_REG_BASE[5] = 0x00000700;     
	MEM_REG_BASE[6] = 0x00000700;    
	MEM_REG_BASE[7] = 0x00018005;     
	MEM_REG_BASE[8] = 0x00018005;     
	MEM_REG_BASE[9] = 0x008C07A3;     
	MEM_REG_BASE[10] = 0x000000B1;   
	MEM_REG_BASE[11] = 0x00000030;     
	MEM_REG_BASE[12] = 0x00000030;    
 }
 
 void clock_init()
 {
 
    CLK_REG_BASE [CLKDIVN]  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1
 //  CLK_REG_BASE [LOCKTMIE] = 0xffffffff; 

	__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        
    "orr    r1, r1, #0xc0000000\n"        
    "mcr    p15, 0, r1, c1, c0, 0\n"        
    );
     CLK_REG_BASE[MPLLCON] = MPLL_400MHZ;  
 }
 
static int is_boot_nor()
{
	#define TEST (*(volatile int *)0)
	TEST = 0xabcdef;
	
	if (TEST == 0xabcdef)
		return 0;
	else
		return 1;
}

void clear_bss(void)
{
        extern int __bss_start, __bss_end;
        int *p = &__bss_start;

        for (; p < &__bss_end; p++)
                *p = 0;
}

void copy2ram(char *src, char *dest, unsigned long len)
{
    unsigned long cnt;
    for (cnt = 0; cnt < len; cnt++){
        dest[cnt] = src[cnt];
    }
}

void disable_wdg(void)
{
    #define WTDG_BASE (*(volatile unsigned long *)0x53000000)
    WTDG_BASE = 0;
}
