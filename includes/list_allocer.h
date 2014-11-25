#ifndef __LIST_ALLOCER_H__
#define __LIST_ALLOCER_H__
#include <types.h>
#include <linux/list.h>
typedef struct{
    u32 flag;
    u32 start;
    struct list_head list; 	
}header;

extern void* list_mem_alloc(struct list_head*, struct list_head *, u32 );
extern int list_mem_free(struct list_head *,struct list_head* ,void *);
extern int list_mem_omb(struct list_head*, struct list_head *);

#endif
