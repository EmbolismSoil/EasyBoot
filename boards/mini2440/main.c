#include <driver.h>
int main()
{
  board_t *mini2440 = board_req("mini2440");
  int ret = register_device(mini2440,CPU,"LED", "LED");/*在mini2440这块板子上使用LED驱动注册一个CPU类型的LED设备*/
  if(!ret) open("/mini2440/CPU/LED");                /*打开mini2440的LED，设备，因为LED被实现为CPU类型的驱动，
						    所以这里要在mini2440/CPU下打开*/
}
