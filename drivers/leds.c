#include <driver.h>
#include <debug.h>
#define GPBCON      (*(volatile unsigned long *)0x56000010)
#define GPBDAT      (*(volatile unsigned long *)0x56000014)

/*
 * LED1,LED2,LED4对应GPB5、GPB6、GPB7、GPB8
 */
#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

static void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

static int open(void *unuse, void *unuse2)
{
	unsigned long i = 0;

	// LED1,LED2,LED4对应的4根引脚设为输出
	GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out;

//	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1,2,3,4
		if(++i == 16)
			i = 0;
//	}

	return 0;
}

static int  write_led(unsigned int addr ,void *buf, unsigned int len)
{
    PUT_STR((char*)buf);
}
d_ops led_ops = {.open = open,.write = write_led};

DRIVER_EXPORT(LED, CPU, NULL, &led_ops, ((void *)0) );
