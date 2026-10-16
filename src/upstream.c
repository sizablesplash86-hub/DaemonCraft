// File 4

#include "config.h"

void upstream(int sockfd)
{
  printf("Running loop...\n");
  while (1)
  {
    int client_fd = accept(sockfd, NULL, NULL);  // C.56.5.2 p.1157

    if (client_fd < 0)
    {
      printf("Failed to accept socket\n");
      return;
    }
/*
    char request[1024];
    read(client_fd, request, sizeof(request) - 1);

    char method[16], uri[256], protocol[16];
    sscanf(request, "%s %s %s", method, uri, protocol);

    char file_to_serve[1024];
    if (strcmp(uri, "/") == 0)
    {
      snprintf(file_to_serve, sizeof(file_to_serve), "%s/index.html", root);
    }
    else 
    {
      size_t len = strlen(uri);
      if (uri[len - 1] == '/') snprintf(file_to_serve, sizeof(file_to_serve), "%s%sindex.html", root, uri);
      else snprintf(file_to_serve, sizeof(file_to_serve), "%s%s", root, uri);
    }
*/

    char file_to_serve[1024];
    snprintf(file_to_serve, sizeof(file_to_serve), "%s/index.html", root);


    char buffer[1024];
    FILE *fts = fopen(file_to_serve, "r");
    if (fts != NULL)
    {
      write(client_fd, HEADER, strlen(HEADER));  // write is a unistd command   strlen(); is a string command

      size_t bytes_read;

      while ((bytes_read = fread(buffer, 1, sizeof(buffer), fts)) > 0) write(client_fd, buffer, bytes_read);  // fread(); is a unistd cmd
    }
    else
    {
      write(client_fd, NOT_FOUND, strlen(NOT_FOUND));
    }
    fclose(fts);
    close(client_fd);
  } // end of loop
}