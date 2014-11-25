#ifndef __MM_H__
#define __MM_H__
#include <types.h>
#include <linux/list.h>
#include <config.h>
typedef enum{ list_t, stree_t }mm_type;
typedef struct _manager{
   /*memory sizes which the manager keep*/
    u32 start;
    u32 end;
    
    void* (*alloc)(struct list_head*, struct list_head* , u32);
    int (*free)(struct list_head *, struct list_head * ,void *);
    int (*omb)(struct list_head* ,struct list_head*);
    
    struct list_head free_list;
    struct list_head alloc_list;
    
    struct list_head free_tree;
    struct list_head alloc_tree; 	 	
}manager;

extern void* malloc(u32 size);
extern void* heap_start(void);
extern void* heap_end(void);
extern int heap_init(mm_type ); 
extern int free(void *ptr);

#endif
