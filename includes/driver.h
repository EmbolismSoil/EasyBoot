/*
 *  header of EasyBoot driver system 
 *
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

#ifndef __DRIVER_H
#define __DRIVER_H
#include <linux/list.h>


#define MAX_DEV 25
#define MAX_BOARD 10

typedef enum{CPU,RAM,NOR,NAND}class_t;
typedef struct __device device_t;
extern device_t __driver_start;
extern device_t __driver_end;

typedef struct{
	 char* (*show)(void *,void *,int );
	 char *info;
}attribute;

typedef struct{
   int (*open)(void *arg1, void *arg2);
   int (*write)(unsigned int addr ,void *buf, unsigned int len);
   int (*read)(unsigned int addr,void *buf, unsigned int len);
   int (*ioctl)(void *arg, int cmd, int cmdarg);
   int (*close)(void *arg);
}d_ops;

struct __device{
    char *type;
    char *drv;/*这个设备使用的驱动名称*/
    class_t class;
    d_ops  ops;
    attribute atr;
    struct list_head list;
    int _flag;
    void  *_pri;
};

typedef struct{
    char *type;
    char *info;
    struct list_head  CPU_list;
    struct list_head  RAM_list;
    struct list_head  NOR_list;
    struct list_head  NAND_list;
    device_t     *open[MAX_DEV];
    attribute atr;
    void *	pri;
}board_t;

extern board_t *board_req(char *type);
extern  int register_device(board_t *board,char driver_type[], char device_type[]);
extern int device_open(char *path);
extern int device_read(int fd, unsigned int addr, void *buf ,unsigned int len);
extern int device_write(int fd, unsigned int addr,void *buf ,unsigned int len);
extern int device_close(board_t *board, int fd);
extern int device_ioctl(board_t *board, int fd, int cmd, int arg);

#define IN_SECTION_DEVICE  __attribute__ ((unused,section (".__easy_boot_driver")))
#define DRIVER_EXPORT(__type, __class, __atr, __ops, __pri ) \
	device_t driver_##__type   IN_SECTION_DEVICE = { \
		.drv = #__type, \
		.class = __class, \
		.atr = __atr,  \
		.ops = __ops, \
		.pri = __pri, \
	}
#endif
