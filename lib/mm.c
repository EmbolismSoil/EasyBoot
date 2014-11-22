/*
 *  EasyBoot memory manage system
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
#include <types.h>
#include <config.h>
#include <linux/list.h>
#include <debug.h>

typedef struct{
    u32 flag;
    u32 start;
    struct list_head list; 	
}header;

typedef struct _manager{
    u32 start;
    u32 end;
    
    void* (*alloc)(struct _manager* mm,u32 size);
    int (*free)(struct _manager* mm,void *ptr);
    int (*omb)(struct _manager* mm);
    
    struct list_head free_list;
    struct list_head alloc_list; 	 	
}manager;
 
static void *mem_find(manager* mm,u32 size)
{
	DEBUG_CHECK((!mm || !size),NULL);
#ifdef DEBUG
	printf("\n\rinto function : ");
	printf(__func__);
	printf("\n\rthe size is : 0x%x\n\r\n\r",size);
	
#endif
	header *pos;
    header *new;
	void *ret;
	
	list_for_each_entry(pos, &mm->free_list, list)
        if ((pos->flag & 0xffffffc) >= size){
		   ret = (void*)(pos->start + sizeof(header));
#ifdef DEBUG
		   printf(" .... find block\n\r");
		   printf("pos flag is : 0x%x\n\r",pos->flag);
		   printf("pos start is : 0x%x\n\r",pos->start);
		   printf("size of header is : 0x%x\n\r",sizeof(header));
		   printf("ret is : 0x%x\n\r",(u32)ret);
#endif
   
    	   new = (header *)(pos->start + size);
    	   new->start = pos->start + size;
    	   new->flag = (pos->flag & 0xfffffffc) - size;
    		
    	   pos->flag = (size | 1);
    	   list_move_tail(&pos->list,&mm->alloc_list);
		   list_add_tail(&new->list, &mm->free_list);


#ifdef DEBUG

           printf("the new addr is : 0x%x\n\r",(u32)new);
           printf("the new start is : 0x%x\n\r",(u32)new->start);
           printf("the new flag is : 0x%x\n\r",(u32)new->flag);

//        header *debug_pos;
//        list_for_each_entry(debug_pos, &mm->free_list, list){
//                printf("new pos addr : 0x%x\n\r",(u32)debug_pos);
//        }
#endif
	
		
		   return ret;
       }
    return NULL;
} 
 
static void* mem_alloc(manager* mm, u32 size)
{
	DEBUG_CHECK(!mm,NULL);
#ifdef DEBUG
	printf("into function : ");
	printf(__func__);
#endif
	size = (size + sizeof(header)) + ((size + sizeof(header)) % 4);

	void *ret = mem_find(mm,size);
#ifdef DEBUG
	printf("\n\rthe size is : 0x%x\n\r",size);
	printf("the ret is : 0x%x\n\r",ret);
#endif
    
    if (!ret){
	printf("exec omb \n\r");
    	mm->omb(mm);
    }
    else
    	return ret;
    	
    if(!(ret = mem_find(mm,size)))
    	return NULL;
    else
    	return ret;	
}

static int mem_free(manager *mm,void *ptr)
{
	DEBUG_CHECK(!ptr,-1);
#ifdef DEBUG
	printf("into function : ");
	printf(__func__);
#endif

	header *pos_alloc;
	header *pos_free;
	header *tmp;
	
	list_for_each_entry(pos_alloc, &mm->alloc_list, list)
		if ((u32)ptr >= pos_alloc->start && 
				(u32)ptr <= pos_alloc->end){
			break;
		}
		
	list_for_each_entry(free_alloc, &mm->free_list, list)
		if (pos_alloc->start > pos_free->start){
			tmp = list_entry(pos_free->list.prev, 
							typeof(*pos_free), list)
			if (pos_alloc->start < tmp->start){
				list_insert(&pos_alloc->list,&pos_free->list);
				return 0;
			}
		}	
	
			
	return -1;
}

static int mem_omb(manager* mm)
{
	return 0;
}
extern void _start(void);
extern u32 __stack;

#define HEAP_BASE ((u32)&__stack + STACK_SIZE + KEEP_SIZE)
static manager heap_manager = {
	.start =  HEAP_BASE + (HEAP_BASE % 4),
	.end = MEM_BASE + MEM_SIZE - KEEP_SIZE,
	.alloc = mem_alloc,
	.free = mem_free,
	.omb = mem_omb,
};

static int _heap_init(manager *mm)
{
    header *tmp = (header *)HEAP_BASE;
    tmp->flag = ( (mm->end - mm->start) | 0);
    tmp->start = mm->start;
    
    INIT_LIST_HEAD(&mm->free_list);
    INIT_LIST_HEAD(&mm->alloc_list);
    
    list_add(&tmp->list, &mm->free_list);

    return 0;
}

int heap_init()
{
    return _heap_init(&heap_manager);
}

void *heap_start()
{
	return (void*)heap_manager.start;
}
void *heap_end()
{
	return (void*)heap_manager.end;
}

void *malloc(u32 size)
{
#ifdef DEBUG
	printf("into function : ");
	printf(__func__);
#endif
	void *ret;
	ret = heap_manager.alloc(&heap_manager,size);
	return ret;
}
 
 
 
 
