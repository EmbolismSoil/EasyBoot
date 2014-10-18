#include "list.h"
#define MAX_DEV

typedef enum{CPU,RAM,NOR,NAND}class_t;
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
    void     *pri;
}device_node;

struct __board{
    list_head	 CPU;
    list_head    RAM;
    list_head    NOR;
    list_head	 NAND;
    driver_list  device[MAX_DEV];
    int (*show)(void *,void *);
    void *	pri;
}; 

