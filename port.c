#include <stdio.h>
#include "core.h"

#define FILE_SIZE 1024
#define STR_LEN 256

void findport(void)
{
  printf("Opening config...\n");
  snprintf(conf, sizeof(conf), "/home/daemoncraft/config/main.conf");
  FILE *pt = fopen(conf, "r");
  if (pt == NULL) perror("Could not find the main config file\n");
  char line[FILE_SIZE];
  int found = 0;
  while (fgets(line, sizeof(line), pt) != NULL)
  {
    if (strncmp(line, "listen *", 6) == 0)
    {
      found = 1;
      break;
    }
  }
  fclose(pt);

  char *port = line + 6;
  if (found)
  {
    line[strcspn(line, "\r\n;")] =0;
    
    while (*port == ' ' || *port == '\t') port++;
    printf("Port %s found!\n\n", port);
  }

  else 
  {
    printf("\nPort not found\n\n");
    return;
  }
  // Inside findport() after cleaning up the port string:
  snprintf(conf_port, sizeof(conf_port), "%s", port);
}