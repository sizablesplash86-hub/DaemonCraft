#include "core.h"

void run_daemoncraft_monolith(int sockfd)
{
  printf("\nDaemonCraft monolith active! Web upstream + CLI console running.\n");
  printf("Type commands below (e.g., 'stop [instancename]'):\nOr run 'exit' to stop DaemonCraft\n>>> ");
  fflush(stdout);

  while (1)
  {
    fd_set read_fds;
    FD_ZERO(&read_fds);
        
    // Monitor standard input (keyboard / terminal console)
    FD_SET(STDIN_FILENO, &read_fds);
        
    // Monitor the web server listening socket for incoming browser connections
    FD_SET(sockfd, &read_fds);

    int max_fd = (sockfd > STDIN_FILENO) ? sockfd : STDIN_FILENO;

    // select() blocks until activity happens on EITHER the web socket or the keyboard
    int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

    if (activity < 0)
    {
      perror("select error");
      break;
    }

    // 1. Check if there is keyboard input in the terminal console
    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      char cmd[256];
      if (fgets(cmd, sizeof(cmd), stdin) != NULL)
      {
        // Remove trailing newline
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strncmp(cmd, "stop ", 5) == 0)
        {
          char *instance_name = cmd + 5;
          printf("Stopping instance: %s...\n", instance_name);
          // TODO: Send graceful stop to that specific Minecraft process pipe
        } 
        else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
        {
          printf("Shutting down DaemonCraft monolith.\n");
          break;
        }
        else if (strlen(cmd) > 0) printf("Unknown command: '%s'. Try 'stop [name]'.\n", cmd);
      }
      printf(">>> ");
      fflush(stdout);
    }

    // 2. Check if a web browser is trying to connect to your upstream
    if (FD_ISSET(sockfd, &read_fds))
    {
      int client_fd = accept(sockfd, NULL, NULL);
      if (client_fd >= 0)
      {
        char request[1024];
        read(client_fd, request, sizeof(request) - 1);

        // Quick router check for your web UI / API
        // Inside your select loop where POST /api/create is handled:
        if (strncmp(request, "POST /api/create", 16) == 0)
        {
          // Find the body of the POST request (after \r\n\r\n)
          char *body = strstr(request, "\r\n\r\n");
          if (body != NULL)
          {
            body += 4; // Skip the blank lines
                
            // Example: Parsing simple URL-encoded data or JSON parameters sent by JS
            // (For testing, you can extract the instance name or pass parameters here)
            char inst_name[128] = "DefaultServer";
                
            // Simple search example for form parsing
            char *name_ptr = strstr(body, "name=");
            if (name_ptr) sscanf(name_ptr, "name=%127[^&]", inst_name);

            // Trigger the folder and config creation!
            create_minecraft_instance(inst_name, 25565, "java", "vanilla-26.3");
          }

            // Respond back to the browser so it knows it succeeded
            char *res = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"status\":\"success\"}";
            write(client_fd, res, strlen(res));
        }
        else
        {
          // Serve index.html dashboard properly
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
            while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), fts)) > 0)
            {
              write(client_fd, file_buffer, bytes_read);
            }
            fclose(fts);
          }
          else
          {
            char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Index Not Found";
            write(client_fd, not_found, strlen(not_found));
          }
        }
        close(client_fd);
      }
    }
  }
}
