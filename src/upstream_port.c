// File 2

#include "config.h"

void upstream_port(void)
{
  printf("Opening config...\n");

  FILE *fp = fopen(conf_file, "r");
  if (fp == NULL)
  {
    perror("Failed to open config file");
    return;
  }

  char line[1024];
  int found = 0;
  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if (strncmp(line, "listen ", 7) == 0)
    {
      found = 1;
      break;
    }
  }
  fclose(fp);

  char *port = line + 7;
  if (found)
  {
    line[strcspn(line, ";\r\n")] = 0;

    printf("Starting DaemonCraft upstream on port %s...\n", port);
    snprintf(conf_port, sizeof(conf_port), "%s", port);

    ip_bind();
  }

  else
  {
    printf("Port not found\n");
    return;
  }
}