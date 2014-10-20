#include <linux/list.h>
#include <debug.h>
#include <linux/stddef.h>
#include <advstr.h>

#define MAX_DEV 25
#define MAX_BOARD 10

typedef enum{CPU,RAM,NOR,NAND}class_t;

typedef struct{
   int (*open)(void *arg1, void *arg2);
   int (*write)(unsigned int addr ,void *buf, unsigned int len);
   int (*read)(unsigned int addr,void *buf, unsigned int len);
   int (*ioctl)(void *arg, int cmd, int cmdarg);
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

static device_t device_pool[MAX_DEV];
static board_t board_pool[MAX_BOARD];
/*
* @get_device: 获取一个型号为type的设备，
* 如果没有在设备池内发现对应型号的设备则
* 返回一个空设备，用户需要自行初始化
*/
device_t *get_device(char *type)
{
    DEBUG_CHECK(!type,NULL);
    char cnt;
    for (cnt = 0;cnt < MAX_DEV && 
		device_pool[cnt].type != NULL; cnt++)
        if(!strcmp(type,device_pool[cnt].type))
	    return &device_pool[cnt];
    
    device_pool[cnt].type = type;
    INIT_LIST_HEAD(&device_pool[cnt].list);
    return &device_pool[cnt];
}

/*
*@ get_board : 获取一个型号为type的board,如果没有在boar_pool
* 中找到对应型号的设备则返回一个空设备，用户需要自行初始化
*/
board_t *get_board(char *type)
{
    DEBUG_CHECK(!type, NULL);
    char cnt;
    for (cnt = 0; cnt < MAX_BOARD && 
	board_pool[cnt].type != NULL; cnt++)
	if (!strcmp(board_pool[cnt].type, type))
	    return &board_pool[cnt];

    board_pool[cnt].type = type;
    INIT_LIST_HEAD(&board_pool[cnt].CPU_list);
    INIT_LIST_HEAD(&board_pool[cnt].RAM_list);
    INIT_LIST_HEAD(&board_pool[cnt].NOR_list);
    INIT_LIST_HEAD(&board_pool[cnt].NAND_list);

    return &board_pool[cnt];
}

#define foreach_class(pos ,board, class) \
	list_for_each_entry(pos, &((board)->class##_list), list) 
	 
/*
* @add_device : 往板子上添加一个class类型的设备device
*/
int add_device(board_t *board, class_t class, device_t *device)
{
	DEBUG_CHECK(!board && !device, -1);
	device_t *pos;
    switch (class){
    case CPU :  
      foreach_class(pos, board, CPU)
        if (!strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->CPU_list);
      break;
	
    case RAM :  
      foreach_class(pos, board, RAM)
        if (!strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->RAM_list);
      break;

    case NOR :  
      foreach_class(pos, board, NOR)
        if(!strcmp(pos->type, device->type))
            return -1;
      list_add(&device->list, &board->NOR_list);
      break;
	
    case NAND : 
      foreach_class(pos, board, NAND)
	if (!strcmp(pos->type, device->type))
	    return -1;
      list_add(&device->list, &board->NAND_list);
      break;
    }
	return 0;
}

#define split_path(__path,__board,__class,__device) do{\
  str_split(__path,__board,'/',1,2); \
  str_split(__path,__class,'/',2,3);\
  strdel_head(__path,__board,'/',3);}while(0)

#define FIND_DEVICE(__nb, __nd, __device_path, __class) \
do{\
       foreach_class(pos, &board_pool[__nb], __class) \
         if (!strcmp(pos->type, __device_path)){ \
           board_pool[__nb].open[__nd] = pos; \
           pos->ops.open(&board_pool[__nb],pos); \
        } \
}while(0)

#define BOARD_MASK 8
#define GET_FD(__nb,__nd) (((__nb) << BOARD_MASK) | (__nd & 0x00ff))


/*
* @device_open : 设备打开函数，
*/

int device_open(char *path)
{
    int cnt;
    int nb = -1;
    int nd = -1;

    device_t  *pos;
    char board[20] , class[20], device[20];
    split_path(path,board,class,device);
   
    for (cnt = 0; cnt < MAX_BOARD; cnt++){
       if (!strcmp(board_pool[cnt].type, board))
	   nb = cnt;
    }
    if (nb < 0) return -1;

    for (cnt = 0; cnt < MAX_DEV; cnt++){
       if (!strcmp(board_pool[nb].open[cnt]->type, device))
		return GET_FD(nb,nd);
       if (!board_pool[nb].open[cnt]->type)
		nd = cnt;
    }
   
  if (strcmp(class,"CPU")){  
     FIND_DEVICE(nb, nd, device, CPU);
     return GET_FD(nb,nd);
  }else if (strcmp(class, "RAM")){
     FIND_DEVICE(nb, nd, device, RAM);
     return GET_FD(nb,nd);
  }else if (strcmp(class, "NOR")){
     FIND_DEVICE(nb, nd, device, NOR);
     return GET_FD(nb,nd);
  }else if (strcmp(class, "NAND")){
     FIND_DEVICE(nb, nd, device, NAND); 
     return GET_FD(nb,nd);
  }else{
     return -1;
  }

}

/*
* @device_open
*/

/*
* @device_read :设备读函数
*/

int device_read(int fd, unsigned int addr, void *buf ,unsigned int len)
{
    int nb = (fd >> 8) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.read(addr, buf, len);
}

/*
* @device_write : 设备写函数
*/

int device_write(int fd, unsigned int addr,void *buf ,unsigned int len)
{
    int nb = (fd >> 8) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.write(addr, buf, len);
}

/*
* @device_close : 关闭设备
*/

int device_close(board_t *board, int fd)
{
    int nb = (fd >> 8) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.close(&board_pool[nb]);
}

/*
* @device_ioctl : 设备控制 
*/
int device_ioctl(board_t *board, int fd, int cmd, int arg)
{
    int nb = (fd >> 8) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.ioctl(board,cmd,arg);
}
