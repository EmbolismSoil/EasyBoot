#ifdef __cplusplus
extern "C"{
#endif
#include <string.h>

/*
*@function : compute the fail function of the pattern
*/
void fail(pattern *pat)
{
    int i = 0,j = 0;
    int len = strlen(pat->str);
#ifdef CHANGE_FAILE
   
    for (j = 1; j < len; j++){
      if (pat->str[i] == pat->str[j]){
          pat->filure[j] = pat->filure[j-1]+1;
	  i++;
      }else{
          pat->filure[j] = -1;
	  i = 0;
      }
    }
#else
    pat->filure[0] = -1;
    for (j = 1; j < len; j++){
      i = pat->filure[j-1];
      while((pat->str[j] != pat->str[i+1]) && (i >= 0))
        i = pat->filure[i];
	if(pat->str[j]  == pat->str[i+1])
	  pat->filure[j] = i + 1;
	else pat->filure[j] = -1;
    } 
#endif
}


#ifdef __cplusplus
}
#endif
