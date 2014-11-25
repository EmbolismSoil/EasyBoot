/*
 *  EasyBoot allocer base on list
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
#include <list_allocer.h>
#include <printf.h>

/*
 * @function : this function can find out the block
 * which can fit the requested sizes whit 10 bytes header.
 * The block size should be four byte aligned.
 */
static void *list_mem_find(struct list_head *free_list,
					struct list_head *alloc_list,const u32 size)
{
	DEBUG_CHECK((!free_list || !size || !alloc_list),NULL);
#ifdef DEBUG
	printf("\n\rinto function : ");
	printf(__func__);
	printf("\n\rthe size is : 0x%x\n\r\n\r",size);
	
#endif
	header *pos;
    header *new;
	void *ret;
	/**/
	list_for_each_entry(pos, free_list, list)
        if ((pos->flag & 0xffffffc) >= size){
		   ret = (void*)(pos->start + sizeof(header));
#ifdef DEBUG
		   printf("pos start is : 0x%x\n\r",pos->start);
		   printf("size of header is : 0x%x\n\r",sizeof(header));
		   printf("ret is : 0x%x\n\r",(u32)ret);
#endif
   
    	   new = (header *)(pos->start + size);
    	   new->start = pos->start + size;
    	   new->flag = (pos->flag & 0xfffffffc) - size;
    		
    	   pos->flag = (size | 1);
    	   list_move_tail(&pos->list,alloc_list);
    	   if (list_empty(free_list))
		        list_add_tail(&new->list, free_list);


#if DEBUG
           printf("the new addr is : 0x%x\n\r",(u32)new);
           printf("the new start is : 0x%x\n\r",(u32)new->start);
           printf("the new flag is : 0x%x\n\r",(u32)new->flag);
#endif
	
		
		   return ret;
       }
    return NULL;
} 
 
void* list_mem_alloc(struct list_head *free_list,
		struct list_head *alloc_list,u32 size)
{
	DEBUG_CHECK(!free_list || !alloc_list,NULL);
#ifdef DEBUG
	printf("into function : ");
	printf(__func__);
#endif
	size = (size + sizeof(header)) + ((size + sizeof(header)) % 4);

	void *ret = list_mem_find(free_list,alloc_list,size);
#ifdef DEBUG
	printf("the ret is : 0x%x\n\r",ret);
#endif
    
    return ret;
}

int list_mem_free(struct list_head *free_list,
		struct list_head *alloc_list,void *const ptr)
{
	DEBUG_CHECK(!ptr,-1);
#ifdef DEBUG
	printf("into function : ");
	printf(__func__);
	printf("ptr 0x%x need to be free\n\r",(u32)ptr);
#endif

	header *pos_alloc;
	header *pos_free;
	header *tmp;
	
	list_for_each_entry(pos_alloc, alloc_list, list){
		if ((u32)ptr >= pos_alloc->start && (u32)ptr <= 
			pos_alloc->start + (pos_alloc->flag & 0xffffffc)){
#ifdef DEBUG
		    printf("\n\rfind out the block will be free : from 0x%x to 0x%x\n\r ",pos_alloc->start,
		    						pos_alloc->start + pos_alloc->flag - 1);
#endif
			break;
		}
	}
	
#ifdef DEBUG 
		printf("\n\rpos_alloc addr : 0x%x",(u32)pos_alloc);
		printf("\n\rpos_alloc->start : 0x%x",(u32)pos_alloc->start);
		printf("\n\rpos_alloc->start + (pos_alloc->flag & 0xffffffc) : 0x%x\n\r",
						pos_alloc->start + (pos_alloc->flag & 0xffffffc));
#endif
	list_for_each_entry(pos_free, free_list, list){
#ifdef DEBUG
		printf("\n\rpos_free addr : 0x%x",(u32)pos_free);
		printf("\n\rpos_free->start : 0x%x",(u32)pos_free->start);
		printf("\n\rpos_free->start + (pos_free->flag & 0xffffffc) : 0x%x\n\r",
						pos_free->start + (pos_free->flag & 0xffffffc));
#endif
		if (pos_alloc->start < pos_free->start){
			if (pos_free->list.prev != free_list){
			    tmp = list_entry(pos_free->list.prev,typeof(*pos_free), list);
#ifdef DEBUG
		printf("pos_free->list.prev 0x%x\n\r",(u32)pos_free->list.prev );
		printf("pos_free->list0x%x\n\r",(u32)(&pos_free->list) );
		printf("\n\rtmp addr : 0x%x",(u32)tmp);
		printf("\n\rtmp->start : 0x%x",(u32)tmp->start);
		printf("\n\rtmp->start + (pos_free->flag & 0xffffffc) : 0x%x\n\r",
						tmp->start + (tmp->flag & 0xffffffc));
#endif					        
			    if (pos_alloc->start > tmp->start){
#ifdef DEBUG
					printf(__LINE__);
				    printf("171 : free memory from 0x%x to 0x%x\n\r",(u32)ptr,
						    (u32)pos_alloc->start + (pos_alloc->flag &=  0xffffffc));
#endif
				    list_del(&pos_alloc->list);
				    list_insert(&pos_alloc->list,&pos_free->list);
				    return 0;
			    }else return -1;
			    
			}else{
#ifdef DEBUG
			printf(__LINE__);
			printf("182 : free memory from 0x%x to 0x%x\n\r",(u32)ptr,
						(u32)pos_alloc->start + (pos_alloc->flag &=  0xffffffc));
#endif
			list_del(&pos_alloc->list);
			list_insert(&pos_alloc->list,&pos_free->list);
			return 0;
			}
		}	
	}
	return -1;
}

int list_mem_omb(struct list_head *free_list,
		struct list_head *alloc_list)
{
	return 0;
}
