/*
 * cpu/s3c2440/start.S
 * Copyright 2014 Lee <ToufuSoil@163.com>
 * 
 * Entry code of the EastBoot for S3C2440 CPU
 */
#define MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define WTREG_BASE	0x53000000
#define SDRAM_BASE	0x30000000

.global _start
_start:
	ldr	 r0,=WTREG_BASE
	mov	 r1,#0
	str	 r1,[r0]

	ldr	sp,=SDRAM_BASE
	bl 	setupram
	bl 	diswtdog
	 /*
         * set the cpu to SVC32 mode
         */
        mrs     r0, cpsr
        bic     r0, r0, #0x1f
        orr     r0, r0, #0xd3
        msr     cpsr, r0

	
