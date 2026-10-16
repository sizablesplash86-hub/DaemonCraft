// File 1

#include "config.h"

char root[64];
char conf_file[128];
char conf_port[10];

int main(void)
{
  printf("Welcome to DaemonCraft %s!\n\n", DAEMONCRAFT_VRS);

  snprintf(root, sizeof(root), "/home/daemoncraft");
  snprintf(conf_file, sizeof(conf_file), "%s/config/main.conf", root);

  upstream_port();
}