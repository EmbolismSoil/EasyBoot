#include "list.h"
typedef enum{CPU，RAM，NOR，NAND}class_t;
typedef struct __board board_t;

typedef struct{
   int (*open)(board_t *board, class_t class, const char *name);
   int (*write)(board_t *board,int fd,unsigned int addr ,void *buf, unsigned int len);
   int (*read)(board_t *board, int fd, unsigned int addr,void *buf, unsigned int len);
   int (*ioctl)(board_t *board, int fd, void *buf, unsigned long cmd);
   int (*close)(board_t *board, int fd);
}d_ops;

typedef struct{
    const char *name;
    d_ops	ops;
}driver_node;

typedef struct{
    driver_node node;
    head_list	d_list;
}driver_list;

struct __board{
    driver_list  *CPU;
    driver_list  *RAM;
    driver_list	 *NOR;
    driver_list	 *NAND;
    driver_node  *device;
    void *	pri;
}; 

