#include <stdio.h>
#include "core.h"

char web_path[STR_LEN];
char conf[STR_LEN];
char conf_port[STR_LEN];
char lan_ip[STR_LEN];

int main()
{
  printf("Example DaemonCraft upstream\n");
  ip();
  findport();
  printf("visit \033[34mhttp://%s:%s\033[0m in your browser", lan_ip, conf_port);
  web();
}