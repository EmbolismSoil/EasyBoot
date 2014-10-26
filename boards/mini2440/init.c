
#define 	WTCON				(*(volatile unsigned long *)0x53000000)

#define 	MEM_CTL_BASE		0x48000000
 
void disable_watch_dog();
void memsetup();

void disable_watch_dog()
{
	WTCON	= 0;
}

void memsetup()
{
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;
	p[0] = 0x22011110;     //BWSCON	
	p[1] = 0x00000700;     //BANKCON0	
	p[2] = 0x00000700;     //BANKCON1	
	p[3] = 0x00000700;     //BANKCON2	
	p[4] = 0x00000700;     //BANKCON3  	
	p[5] = 0x00000700;     //BANKCON4	
	p[6] = 0x00000700;     //BANKCON5	
	p[7] = 0x00018005;     //BANKCON6	
	p[8] = 0x00018005;     //BANKCON7	
	p[9] = 0x008C07A3;     //REFRESH	
	p[10] = 0x000000B1;     //BANKSIZE	
	p[11] = 0x00000030;     //MRSRB6	
	p[12] = 0x00000030;     //MRSRB7

}
