
#include <printf.h>
#include <string.h>
#include "FIFO.h"

/*
* @function : we can pop/push the date in/to the fifo.
* and we should compute the len carefully before use it
* we need to avoid using this function directely.
* and we will proved an MACRO for the user to oparete the FIFO
*/

int push(FIFO  *const fp,void *const date,int len)
{   
    int count;
    /*entrance location arguments checked*/
    if (fp->size - len < 0){
        return -1;
    }else{
        fp->size -= len;
    }
    /*here we push the date*/
    for (count = 0;count < len ;count++){
	if (fp->tp + count > fp->fifo_top)
	  fp->tp = fp->fifo_base;
        *(char *)(fp->tp + count) = *(char *)(date + count);
    }
    /*update the top pointer of the FIFO*/
    fp->tp += count;   
    return 0;
}

int pop(FIFO *const fp, void *date, int len)
{
    int count;
    /*entrance location arguments checked*/
    if ((fp->size + len) > (fp->fifo_top - fp->fifo_base)){
	return -1;
    }else{
    	 fp->size += len;
    }
    /*here we pop the date*/
    for (count = 0;count < len; count++){
        if (fp->bp + count > fp->fifo_top)
	    fp->bp = fp->fifo_base;
         *(char *)(date + count) = *(char *)(fp->bp + count);
    } 
   /*update the bottom pointer of the FIFO */
   fp->bp += count;     
   return 0;
}

/******************************************************************
			Demo
*******************************************************************
void main(void)
{	
    int w_buf[500];
    int r_buf[500];
    int count = 0;
    
    INIT_ARRAY(w_buf);

    memset(r_buf,0,sizeof(r_buf));
    PRINT_ARRAY(r_buf);

    DECLARE_FIFO(test,10);
	
    for (count = 0; count < ARRAY_NUM(w_buf); count++){
       PUSH(test, w_buf[count]);
       POP(test, r_buf[count]);
    }

    PRINT_ARRAY(r_buf);    
}
********************************************************************

*******************************************************************/

