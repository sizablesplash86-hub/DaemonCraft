// File 1

#include "config.h"

char root[64];
char root_insts[128];
char conf_file[128];
char conf_port[10];
char cmd[256];

int main(void)
{
  printf("\nWelcome to DaemonCraft %s!\n\n", DAEMONCRAFT_VRS);

  snprintf(root, sizeof(root), "/home/daemoncraft");
  snprintf(root_insts, sizeof(root_insts), "%s/instances", root);
  snprintf(conf_file, sizeof(conf_file), "%s/config/main.conf", root);

  upstream_port();
}