#include <driver.h>
#include <debug.h>

typedef void (*start_boot)();
start_boot s3c2440_boot = (start_boot*)0x30008000;
int user_main()
{
  int fd_tty;
  int fd_led;

  board_t *mini2440 = board_req("mini2440");
  
  int ret = register_device(mini2440,CPU,"UART", "TTY");
  if(!ret) fd_tty = device_open("/mini2440/CPU/TTY");       

  ret = register_device(mini2440,CPU,"LED","LED");
  if (!ret) fd_led = device_open("/mini2440/CPU/LED");
  
  
  register_device(mini2440,CPU,"NAND","NAND");
  int fd_nand = device_open("/mini2440/CPU/NAND/");

  PUT_DEC(fd_nand);
  unsigned char *kernel = (unsigned char *)0x30008000;
  device_read(fd_nand,0x200000,kernel,0x300000);

  device_write(fd_tty,0,"read kernel...finish\n\r",0);
   s3c2440_boot();
  return;
}
