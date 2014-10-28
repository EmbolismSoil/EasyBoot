/*
 *  EasyBoot driver system
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

#include <linux/list.h>
#include <debug.h>
#include <linux/stddef.h>
#include <advstr.h>
#include <driver.h>



static board_t board_pool[MAX_BOARD];

/*
 * @function : use as an default_show meber of atr.show
 *  copy the atr.info to r_buf and return the r_buf
*/
static char *default_show(void *atr,void *r_buf,int len)
{
   DEBUG_CHECK(!r_buf, NULL);
    r_buf = (void *)(((attribute *)atr)->info);
	return (char *)r_buf;
}
/*
 * @function : request an board from boards pool.
 * System will alloc an blank board if it can not find this type the blank board which 
  * user have been given should be  initialised.his blank board use the function 
  *'defaule_show' as an default member : boaard.show
 */
board_t *board_req(char *type)
{
    DEBUG_CHECK(!type, NULL);
    char cnt;
    for (cnt = 0; cnt < MAX_BOARD && 
	board_pool[cnt].type != NULL; cnt++)
	if (!strcmp(board_pool[cnt].type, type))
	    return &board_pool[cnt];

    board_pool[cnt].type = type;
    board_pool[cnt].atr.show = default_show;
    
    INIT_LIST_HEAD(&board_pool[cnt].CPU_list);
    INIT_LIST_HEAD(&board_pool[cnt].RAM_list);
    INIT_LIST_HEAD(&board_pool[cnt].NOR_list);
    INIT_LIST_HEAD(&board_pool[cnt].NAND_list);

    return &board_pool[cnt];
}

#define foreach_class(pos ,board, class) \
	list_for_each_entry(pos, &((board)->class##_list), list) 
	 
/*
* @add_device : add device to  board.
*/
static int add_device(board_t *board,  device_t *device)
{
	DEBUG_CHECK(!board || !device, -1);
	device_t *pos;
    switch (device->class){
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


/*
 * @function : register an device on board
 *   we get the device object by foreach the .__easy_boot_device section
 *  the symbol '__device_start' just declare here "extern __device_start" 
 */
 
 int register_device(board_t *board, char device_type[])
 {
    device_t *devp;
    
	for (devp = &__device_start; devp != &__device_end; devp++) {
		if (!strcmp(devp->type, device_type))
		    if (!add_device(board, devp)){
		    	return 0;
		    }else{
		    	return -1;
		    }
	}
 }
 
#define split_path(__path,__board,__class,__device) do{\
  str_split(__path,__board,'/',1,2); \
  str_split(__path,__class,'/',2,3);\
  strdel_head(__path,__board,'/',3);}while(0)

#define FIND_DEVICE(__nb, __nd, __device_path, __class) \
do{\
       foreach_class(pos, &board_pool[__nb], __class) \
         if (!strcmp(pos->type, __device_path) && pos->class == __class){ \
           board_pool[__nb].open[__nd] = pos; \
           pos->ops.open(&board_pool[__nb],pos); \
        } \
}while(0)

#define BOARD_MASK 8
#define GET_FD(__nb,__nd) (((__nb) << BOARD_MASK) | (__nd & 0x00ff))


/*
* @device_open 
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
* @device_read 
*/

int device_read(int fd, unsigned int addr, void *buf ,unsigned int len)
{
    int nb = (fd >>  BOARD_MASK) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.read(addr, buf, len);
}

/*
* @device_write 
*/

int device_write(int fd, unsigned int addr,void *buf ,unsigned int len)
{
    int nb = (fd >>  BOARD_MASK) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.write(addr, buf, len);
}

/*
* @device_close 
*/

int device_close(board_t *board, int fd)
{
    int nb = (fd >> BOARD_MASK) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.close(&board_pool[nb]);
}

/*
* @device_ioctl  
*/
int device_ioctl(board_t *board, int fd, int cmd, int arg)
{
    int nb = (fd >>  BOARD_MASK) & 0x00ff;
    int nd = fd & 0x00ff;
    return board_pool[nb].open[nd]->ops.ioctl(board,cmd,arg);
}
