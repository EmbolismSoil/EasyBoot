/*
 *  Copyright (c) 2014	Lee <TofuleeSoil@163.com>
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
#define MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define WTREG_BASE	0x53000000
#define SDRAM_BASE	0x30000000

.global _start
_start:
	ldr	 r0,=WTREG_BASE
	mov	 r1,#0
	str	 r1,[r0]

	ldr	sp,=__stack
//	bl 	setupram
//	bl 	diswtdog
	 /*
         * set the cpu to SVC32 mode
         */
        mrs     r0, cpsr
        bic     r0, r0, #0x1f
        orr     r0, r0, #0xd3
        msr     cpsr, r0

	
