#include <driver.h>
#include <debug.h>
#include <printf.h>

typedef void (*start_boot)();
start_boot s3c2440_boot = (start_boot)0x30008000;

void delay(int x)
{
   int t = 2000000;
   while(t--)
    for (; x > 0; x--);
}
int user_main()
{
  int fd_tty;
  int fd_nand;

  board_t *mini2440 = board_req("mini2440");
  /*must register an TTY device for console*/
  int ret = register_device(mini2440,CPU,"UART", "TTY");
  if(!ret) fd_tty = device_open("/mini2440/CPU/TTY");       

  register_device(mini2440,NAND,"NAND","NAND");
  fd_nand = device_open("/mini2440/NAND/NAND/");

  unsigned char *kernel = (unsigned char *)0x30008000;
  device_read(fd_nand,0x200000,kernel,0x300000);

  printf("\n\rread 0x%x size from nand flash to address : 0x%x... Ok\n\r",0x300000 - 0x200000, kernel);
  printf("\n\rbooting....\n\rfrom : EasyBoot\n\r");
  
  delay(2000000);
  s3c2440_boot(193);
  printf("ERROR : boot faile....\n");
  while(1);
  return;
}



