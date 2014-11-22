#ifndef __MINI2440_H
#define __MINI2440_H
/*
 * @config the stack sizes 
 */
#define SVC_STACK_SIZE 0x35c00
#define IRQ_STACK_SIZE 0x4000
#define FIQ_STACK_SIZE 0x4000
#define BAD_STACK_SIZE 0x4000
#define STACK_SIZE (SVC_STACK_SIZE + IRQ_STACK_SIZE \
		+ FIQ_STACK_SIZE + BAD_STACK_SIZE)
		
#define CONFIG_S3C2440
#define MEM_SIZE (64 * 1024 * 1024)
#define MEM_BASE 0x30000000
#define KEEP_SIZE 0x1000

//#define DEBUG
#endif
