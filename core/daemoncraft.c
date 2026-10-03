#include <daemoncraft/core.h>

char web_path[STR_LEN];
char conf[STR_LEN];
char conf_port[STR_LEN];
char lan_ip[STR_LEN];
char ipv4[STR_LEN];
char cmd[256];

int main(void)
{
  printf("Starting DaemonCraft upstream...\n");
  ip();
  findport();

  pubip();
//  printf("The public IP is %s\n\n", ipv4);      // just here for reference

  printf("visit \033[34mhttp://%s:%s\033[0m in your browser\n", lan_ip, conf_port);

  char jva[STR_LEN];
  char bdrk[STR_LEN];
  snprintf(jva, sizeof(jva), "nc -z -w 1 %s %s >/dev/null 2>&1", ipv4, JAVA);
  snprintf(bdrk, sizeof(bdrk), "nc -z -u -w 1 %s %s >/dev/null 2>&1", ipv4, BDRCK);

  if (system(jva) != 0) printf("\n\033[31mPort %s not open.\033[0m Please do so. Moving on...\n\n", JAVA);
  else printf("\nPort %s open!\n\n", JAVA);

  if (system(bdrk) != 0) printf("\n\033[31mPort %s not open.\033[0m Please do so. Moving on...\n\n", BDRCK);
  else printf("\nPort %s open!\n\n", BDRCK);

//  prtfwd( 25565, 25565);  // uneeded now, but leaving it

  // change in here when testing
  ip_bind();
}