/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  this is just an early version of SpyderFly slightly modified *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "core.h"

#define FILE_SIZE 1024
#define STR_LEN 256

void web(int sockfd)
{
  printf("\nDaemonCraft active on port %s! press ctrl+C to stop\n", conf_port);
  while(1)
  {
    int client_fd = accept(sockfd, NULL, NULL);
    if (client_fd < 0)
    {
      perror("accept failed");
      continue;
    }

    char request[1024];
    read(client_fd, request, sizeof(request) - 1);
    char *root = "/home/daemoncraft/web";

    char file_to_serve[STR_LEN];
    snprintf(file_to_serve, sizeof(file_to_serve), "%s/index.html", root);

    FILE *fts = fopen(file_to_serve, "r");
    if (fts != NULL)
    {
      char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
      write(client_fd, header, strlen(header));

      char file_buffer[1024];
      size_t bytes_read;
      while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), fts)) > 0) write(client_fd, file_buffer, bytes_read);
      printf("Index loaded!\n");
      fclose(fts);
    }
    else
    {
      char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Index Not Found";
      write(client_fd, not_found, strlen(not_found));
      printf("Index not found!\n");
    }
    close(client_fd);
  }  // end of loop
}  // end of main