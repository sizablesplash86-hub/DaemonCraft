// File 1

#include <daemoncraft/core.h>

char web_path[STR_LEN];
char conf[STR_LEN];
char conf_port[STR_LEN];
char lan_ip[STR_LEN];
char ipv4[STR_LEN];
char cmd[256];

int main(void)
{
  printf("Starting DaemonCraft %s upstream...\n", DMC_RLS);
  ip();  // File 2
  findport();  // File 3

  // I don't even think the code needs this still
  pubip();  // File 4

  printf("visit \033[34mhttp://%s:%s\033[0m in your browser\n", lan_ip, conf_port);

//  prtfwd( 25565, 25565);  // uneeded now, but leaving it

  // change in here when testing
  ip_bind();  // File 5
}