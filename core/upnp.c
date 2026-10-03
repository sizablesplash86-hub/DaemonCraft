// code no longer needed, leaving for reference

#include "core.h"

void prtfwd(int external_port, int internal_port)
{
  printf("external port is %d & internal port is %d\n", external_port, internal_port);
  char command[256];

  if (system("upnpc >/dev/null 2>&1") != 0) system("sudo apt install miniupnpc -y");
  snprintf(command, sizeof(command), "upnpc -a 192.168.150.237 %d %d TCP >/dev/null 2>&1", internal_port, external_port);
  int result = system(command);
  if (result == 0) printf("[UPnP] Successfully requested router port forward for port %d!\n", external_port);
  else printf("[UPnP] Failed to configure router port forwarding.\nRouter likely doesn't support it or port is not opened.\nIf port is not opened, do that now.\n");
}