#include "core.h"

char web_path[STR_LEN];
char conf[STR_LEN];
char conf_port[STR_LEN];
char lan_ip[STR_LEN];
char ipv4[STR_LEN];

int main(void)
{
  printf("Starting DaemonCraft upstream...\n");
  ip();
  findport();

  pubip();
//  printf("The public IP is %s", ipv4);      // just here for reference

  printf("visit \033[34mhttp://%s:%s\033[0m in your browser\n", lan_ip, conf_port);

  // change in here when testing
  ip_bind();
}