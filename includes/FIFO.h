#ifndef FIFO_H
#define FIFO_H

/*
*@we need the void pointer to build a genernal FIFO 
*needn't care the type of date
*/
typedef struct{
    void *const fifo_base;
    void *fifo_top;
    void *tp;
    void *bp;
    int size;
}FIFO;

/*
*@define the operation of the FIFO queue
*/
#define DECLARE_FIFO(__name,__fsize) \
	unsigned char __FIFO_array_##__name##__fsize[__fsize];\
FIFO __name = {\
  .fifo_base = __FIFO_array_##__name,\
  .fifo_top = __FIFO_array_##__name + __fsize,\
  .tp = __FIFO_array_##__name,\
  .bp = __FIFO_array_##__name,\
  .size = __fsize, \
}

#define PUSH(fifo_name, date) push(&fifo_name, &date, sizeof(date))
#define POP(fifo_name, date) pop(&fifo_name, &date, sizeof(date))

extern int push(FIFO  *const fp,void *const date,int len);
extern int pop(FIFO *const fp, void *date, int len);


#endif
