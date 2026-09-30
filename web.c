// this is just an early version of SpyderFly slightly modified

#include <stdio.h>
#include "core.h"

#define FILE_SIZE 1024
#define STR_LEN 256

void web(void)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("Socket creation failed\n");
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    perror("setsockopt failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  int port_num = atoi(conf_port);
  addr.sin_port = htons(port_num);
  addr.sin_addr.s_addr = INADDR_ANY;

  socket(AF_INET, SOCK_STREAM, 0);
  int bind_status = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  if (bind_status < 0)
  {
    perror("Failed to bind address\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, 10) < 0)
  {
    perror("Listen failed\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("\nSpyderFly Web server upstream active! press ctrl+C to stop\n");
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