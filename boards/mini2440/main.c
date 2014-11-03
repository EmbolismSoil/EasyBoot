#include <driver.h>
#include <debug.h>
int user_main()
{
  int fd_tty;
  int fd_led;

  board_t *mini2440 = board_req("mini2440");
  
  int ret = register_device(mini2440,CPU,"UART", "TTY");
  if(!ret) fd_tty = device_open("/mini2440/CPU/TTY");       

  ret = register_device(mini2440,CPU,"LED","LED");
  if (!ret) fd_led = device_open("/mini2440/CPU/LED");
  
  device_write(fd_led,0,"led testing....\n\r",0);
  device_write(fd_tty,0,"tty testing...\n\r",0);
  while(1);
}
