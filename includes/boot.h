#ifndef __SET_TAG_H__
#define __SET_TAG_H__

extern void setup_tag(char *);
typedef void (*start_boot)(unsigned int,unsigned int,unsigned int);
extern start_boot s3c2440_boot;

#endif
