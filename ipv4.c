#include <stdio.h>

#include "core.h"

void ipv4(void)
{
  FILE *fp;
  char ip_buffer[64];

  // Open the command for reading
  fp = popen("curl -s -4 ifconfig.me", "r");
  if (fp == NULL)
  {
    perror("Failed to run curl command");
    return;
  }

  // Read the output into the buffer
  if (fgets(ip_buffer, sizeof(ip_buffer), fp) != NULL)
  {
    // Strip the trailing newline character if it exists
    ip_buffer[strcspn(ip_buffer, "\n")] = 0;

    printf("Captured IP Address: %s\n", ip_buffer);
  }
  else fprintf(stderr, "Failed to read output from curl.\n");

  // Close the file pointer
  int status = pclose(fp);
  if (status == -1) perror("Error closing pipe");

  return;
}
