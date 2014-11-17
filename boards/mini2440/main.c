#include <driver.h>
#include <debug.h>
#include <printf.h>
#include <boot.h>

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

  char *cmd_line = "init=/linuxrc console=ttySAC0 root=/dev/nfs rw nfsroot=192.168.1.50:/rootfs ip=192.168.1.10:192.168.1.50:192.168.1.1:255.255.255.0:lee:eth0:off";
  printf("\n\r\n\r\n\rsetup cmd_line: %s",cmd_line);

  setup_tag(cmd_line);

  printf("copying kernel from nand to RAM....\n\r");
  unsigned char *kernel = (unsigned char *)0x30008000;
  device_read(fd_nand,0x200000,kernel,0x300000);

  printf("\n\rread 0x%x size from nand flash to address : 0x%x... Ok\n\r",
		0x300000, kernel);
  printf("\n\rbooting....\n\rfrom : EasyBoot\n\r");
  
  delay(2000000);
  s3c2440_boot(0,193,0x30000100);
  printf("ERROR : boot faile....\n");
  return;
}






