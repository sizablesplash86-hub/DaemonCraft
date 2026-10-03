#include <daemoncraft/core.h>

void pubip(void)
{
  FILE *fp;
  char ip_buffer[64];

  fp = popen("curl -s -4 ifconfig.me", "r");
  if (fp == NULL)
  {
    perror("Failed to run curl command");
    return;
  }

  if (fgets(ip_buffer, sizeof(ip_buffer), fp) != NULL) ip_buffer[strcspn(ip_buffer, "\n")] = 0;
  else fprintf(stderr, "Failed to read output from curl.\n");

  int status = pclose(fp);
  if (status == -1) perror("Error closing pipe");

  snprintf(ipv4, sizeof(ipv4), "%s", ip_buffer);
  return;
}