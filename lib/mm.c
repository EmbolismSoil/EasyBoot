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

#include <mm.h>
#include <list_allocer.h>
//#include <stree_allocer.h>

extern void _start(void);
extern u32 __stack;
#define HEAP_BASE ((u32)&__stack + STACK_SIZE + KEEP_SIZE)

static manager heap_manager = {
	.start =  HEAP_BASE + (HEAP_BASE % 4),
	.end = MEM_BASE + MEM_SIZE - KEEP_SIZE,
};

static int list_heap_init(manager *mm)
{
    header *tmp = (header *)HEAP_BASE;
    tmp->flag = ( (mm->end - mm->start) | 0);
    tmp->start = mm->start;
    mm->alloc = list_mem_alloc;
    mm->free = list_mem_free;
    mm->omb = list_mem_omb;
    
    INIT_LIST_HEAD(&mm->free_list);
    INIT_LIST_HEAD(&mm->alloc_list);
    
    list_add(&tmp->list, &mm->free_list);

    return 0;
}

int heap_init(mm_type type)
{   
	switch(type){
		case list_t : 
			return list_heap_init(&heap_manager);
		case stree_t :
			return ;
	}
    
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
	printf("\n\r");
#endif
	void *ret;
	ret = heap_manager.alloc(&heap_manager.free_list,
						&heap_manager.alloc_list,size);
	if (!ret){
	    heap_manager.omb(&heap_manager.free_list,
	    			&heap_manager.alloc_list);
	    ret = heap_manager.alloc(&heap_manager.free_list,
						&heap_manager.alloc_list,size);
	}
		
	return ret;
}
 
int free(void *ptr)
{
	 return heap_manager.free(&heap_manager.free_list,
	 						&heap_manager.alloc_list,ptr);
}
 
 
