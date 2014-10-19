#ifndef __DRIVER_H
#define __DRIVER_H
#include <linux/list.h>


#define MAX_DEV 25
#define MAX_BOARD 10

typedef enum{CPU,RAM,NOR,NAND}class_t;

typedef struct{
   int (*open)(void *arg1, void *arg2);
   int (*write)(unsigned int addr ,void *buf, unsigned int len);
   int (*read)(unsigned int addr,void *buf, unsigned int len);
   int (*ioctl)(void *arg, int cmd);
   int (*close)(void *arg);
}d_ops;

typedef struct{
    char *type;
    d_ops  ops;
    struct list_head list;
    int _flag;
    void  *_pri;
}device_t;

typedef struct{
    char *type;
    struct list_head  CPU_list;
    struct list_head  RAM_list;
    struct list_head  NOR_list;
    struct list_head  NAND_list;
    device_t     *open[MAX_DEV];
    int (*show)(void *,void *);
    void *	pri;
}board_t;

extern device_t *get_device(char *type);
extern board_t *get_board(char *type);
extern int add_device(board_t *board, class_t class, device_t *device);
extern int device_open(board_t *board, class_t class, char *type);
extern int device_read(board_t *board, int fd, unsigned int addr, 
			     void *buf ,unsigned int len);
extern int device_write(board_t *board, int fd, unsigned int addr,
                             void *buf ,unsigned int len);
extern int device_close(board_t *board, int fd);
extern int device_ioctl(board_t *board, int fd, int cmd, void *arg);
#endif
