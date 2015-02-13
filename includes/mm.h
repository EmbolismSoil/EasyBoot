#ifndef __MM_H__
#define __MM_H__

extern void* malloc(u32 size);
extern int free(void *);
extern void* heap_start(void);
extern void* heap_end(void);
extern int heap_init(void); 

#endif
