#include <linux/list.h>
#include <debug.h>
#include <linux/stddef.h>
#define MAX_DEV 25

typedef enum{CPU,RAM,NOR,NAND}class_t;
typedef struct __board board_t;

typedef struct{
   int (*open)(void *arg1, void arg2);
   int (*write)(unsigned int addr ,void *buf, unsigned int len);
   int (*read)(unsigned int addr,void *buf, unsigned int len);
   int (*ioctl)(void *arg, int cmd);
   int (*close)(void *arg);
}d_ops;

typedef struct{
    char *name;
    d_ops  ops;
    struct list_head list;
    void  *pri;
}device_t;

struct __board{
    struct list_head  CPU_list;
    struct list_head  RAM_list;
    struct list_head  NOR_list;
    struct list_head  NAND_list;
    device_t     open[MAX_DEV];
    int (*show)(void *,void *);
    void *	pri;
};

static device_t device_pool[MAX_DEV];

/*
* @get_device: 获取一个型号为type的设备，
* 如果没有在设备池内发现对应型号的设备则
* 返回一个空设备，用户需要自行初始化
*/
device_t get_device(char *type)
{
    DEBUG_CHECK(!type);
    char cnt;
    for (cnt = 0;cnt < MAX_DEV && 
		device_pool[cnt].name != NULL; cnt++)
        if(strcmp(type,device_pool[cnt].name))
	    return device_pool[cnt];
    
    device_pool[cnt].name = type;
    INIT_LIST_HEAD(&device[cnt].list);
    return device_pool[cnt];
}

#define foreach_class(pos ,board, class) \
	list_for_each_entry(pos, &board->class##_list, list) \
	 
/*
* @add_device : 往板子上添加一个class类型的设备device
*/
int add_device(board_t *board, class_t class, device_t *device)
{
	DEBUG_CHECK(!board && !device);
    switch (class){
    case CPU :  
      foreach_class(pos, board, CPU)
        if (strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->CPU_list);
      break;
	
    case RAM :  
      foreach_class(pos, board, RAM)
        if (strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->RAM_list);
      break;

    case NOR :  
      foreach_class(pos, board, NOR)
        if(strcmp(pos->type, device->type))
            return -1;
      list_add(&device->list, &board->NOR_list);
      break;
	
    case NAND : 
      foreach_class(poss, board, NAND)
	if (strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->NAN_list);
      break;
    }
	return 0;
}

/*
* @device_open : 设备打开函数，
*/

int device_open(board_t *board, class_t class, char *type)
{
    int cnt;
    int fd;
    device_t  *pos;
    for (cnt = 0; cnt < MAX_DEV; cnt++){
       if (strcmp(board->open[cnt].type, type))
		return cnt;
       if (!board->open[cnt].type)
		fd = cnt;
    }

    switch (class){
    case CPU : 
      foreach_class(pos, board, CPU)
        if (strcmp(pos->type, type)){
           board->open[fd] = pos;
	   pos->open(board,pos);
           return fd;
	}

    case RAM :
      foreach_class(pos, board, RAM)
	if (strcmp(pos->type, type)){
	   board->open[fd] = pos;
	   pos->open(board,pos);
	   return fd;
	}

    case NOR : 
      foreach_class(pos, board, RAM)
        if (strcmp(pos->type, type)){
           board->open[fd] = pos;
	   pos->open(board,pos);
           return fd;
        }

    case NAND : 
      foreach_class(pos, board, RAM)
        if (strcmp(pos->type, type)){
           board->open[fd] = pos;
	   pos->open(board,pos);
           return fd;
        }

    }

   return -1;
}

/*
* @device_read :设备读函数
*/

int device_read(board_t *board, int fd, unsigned int addr, 
			     void *buf ,unsigned int len)
{
    return board->open[fd].read(addr, buf, len);
}

/*
* @device_write : 设备写函数
*/

int device_read(board_t *board, int fd, unsigned int addr,
                             void *buf ,unsigned int len)
{
    return board->open[fd].read(addr, buf, len);
}

/*
* @device_close : 关闭设备
*/

int device_read(board_t *board, int fd)
{
    return board->open[fd].close(board);
}

/*
* @device_ioctl : 设备控制 
*/
int device_ioctl(board_t *board, int fd, int cmd)
{
    return board->open[fd].ioctl(board,cmd);
}
