// File 6

#include <daemoncraft/core.h>

void run_daemoncraft_monolith(int sockfd)
{
  printf("\nDaemonCraft Active!\n\n");
  printf("Type commands below (e.g., 'stop [instancename]'):\nOr run 'exit' to stop DaemonCraft\n>>> ");
  fflush(stdout);

  while (1)
  {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(sockfd, &read_fds);
    int max_fd = (sockfd > STDIN_FILENO) ? sockfd : STDIN_FILENO;
    int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

    if (activity < 0)
    {
      perror("select error");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      if (fgets(cmd, sizeof(cmd), stdin) != NULL)
      {
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strncmp(cmd, "start ", 6) == 0)
        {
          char *instance_name = cmd + 6;
          char ls[512];
          snprintf(ls, sizeof(ls), "ls /home/daemoncraft/instances/%s/ >/dev/null 2>&1", instance_name);
          if (system(ls) != 0)
          {
            printf("%s not found\n", instance_name);
            continue;
          }
          printf("Starting instance: %s...\n", instance_name);
          // TODO: Send graceful start to that specific Minecraft process pipe
          start_inst(instance_name);  // File 7
        } 

        if (strncmp(cmd, "stop ", 5) == 0)
        {
          char *instance_name = cmd + 5;
          char ls[512];
          snprintf(ls, sizeof(ls), "ls /home/daemoncraft/instances/%s/ >/dev/null 2>&1", instance_name);
          if (system(ls) != 0)
          {
            printf("%s not found\n", instance_name);
            continue;
          }
          printf("Stopping instance: %s...\n", instance_name);
          // TODO: Send graceful stop to that specific Minecraft process pipe
        } 
        else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
        {
          printf("Stopping DaemonCraft...\n");
          break;
        }

        else if (strncmp(cmd, "create ", 5) == 0)
        {
          char port [6];
          char edition[64];
          char version[STR_LEN];
          char mod[32];
          char min[4];
          char max[4];

          char *instance_name = cmd + 7;
          char ls[512];
          snprintf(ls, sizeof(ls), "ls /home/daemoncraft/instances/%s/ >/dev/null 2>&1", instance_name);
          if (system(ls) == 0)
          {
            printf("%s already exists\n", instance_name);
            continue;
          }

          printf("Creating %s instance...\n", instance_name);
          
          printf("Java or Bedrock (type in lowercase): ");
          fgets(edition, sizeof(edition), stdin);
          edition[strcspn(edition, "\r\n")] = 0;

          if (strcmp(edition, "java") == 0)
          {
            printf("Enter Java version: ");
            fgets(version, sizeof(version), stdin);
            version[strcspn(version, "\r\n")] = 0;

            printf("Enter mod loader (or type vanilla): ");
            fgets(mod, sizeof(mod), stdin);
            mod[strcspn(mod, "\r\n")] = 0;

            printf("Enter minimium ram allocation: ");
            fgets(min, sizeof(min), stdin);
            min[strcspn(min, "\r\n")] = 0;

            printf("Enter maximum ram allocation: ");
            fgets(max, sizeof(max), stdin);
            max[strcspn(max, "\r\n")] = 0;
          }

          if (strcmp(edition, "bedrock") == 0)
          {
            printf("Latest or preview (type in lowercase): ");
            fgets(version, sizeof(version), stdin);
            version[strcspn(version, "\r\n")] = 0;
            snprintf(mod, sizeof(mod), "n/a");
          }

          // fix this part
/*
          else if (strlen(edition) > 0)
          {
            printf("Error! Enter Java or Bedrock\n>>>");
            continue;
          }
*/
          printf("Enter port number: ");
          fgets(port, sizeof(port), stdin);
          port[strcspn(port, "\r\n")] = 0;
            
          create_minecraft_instance(instance_name, port, edition, version, mod, min, max);  // File 7
        }
        else if (strlen(cmd) > 0) printf("Unknown command: '%s'. Try 'stop [name]'.\n", cmd);
      }
      printf(">>> ");
      fflush(stdout);
    }

    if (FD_ISSET(sockfd, &read_fds))
    {
      int client_fd = accept(sockfd, NULL, NULL);
      if (client_fd >= 0)
      {
        char request[1024];
        read(client_fd, request, sizeof(request) - 1);

        if (strncmp(request, "POST /api/create", 16) == 0)
        {
          // this is for the create via web; it needs a rewrite

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

          //  create_minecraft_instance(instance_name, port, edition, version, mod, min, max);   // File 7    // change this for the JS HTML
          }

            // Respond back to the browser so it knows it succeeded
            char *res = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"status\":\"success\"}";
            write(client_fd, res, strlen(res));

          printf("Not working\n");
        }

        else
        {
          char method[16], uri[STR_LEN], protocol[16];
          sscanf(request, "%s %s %s", method, uri, protocol);

          char *root = "/home/daemoncraft";
          char file_to_serve[STR_LEN * 2];

          if (strcmp(uri, "/") == 0) snprintf(file_to_serve, sizeof(file_to_serve), "%s/index.html", root);
          else 
          {
            size_t len = strlen(uri);
            if (uri[len - 1] == '/') snprintf(file_to_serve, sizeof(file_to_serve), "%s%sindex.html", root, uri);
            else snprintf(file_to_serve, sizeof(file_to_serve), "%s%s", root, uri);
          }
          FILE *fts = fopen(file_to_serve, "r");
          if (fts != NULL)
          {
            char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
            write(client_fd, header, strlen(header));

            char file_buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), fts)) > 0) write(client_fd, file_buffer, bytes_read);
            fclose(fts);
          }
          else
          {  // if sub-folder exists & index doesn't, white screen. Fix that somehow later... add a part for auto index
            char *not_found = 
              "HTTP/1.1 404 Not Found\r\n"
              "Content-Type: text/html; charset=UTF-8\r\n"
              "Connection: close\r\n\r\n"
              "<html><head><title>SpyderFly Site Not Found</title></head>"
              "<body><center><h1>SpyderFly Site Not Found</h1></center>"
              "<center>DaemonCraft is a branch of SpyderFly. Visit <a href=\"https://spyderfly.sizablesplash.com/support/\">https://spyderfly.sizablesplash.com/support/</a> for support</center></body></html>";

            write(client_fd, not_found, strlen(not_found));
          }
          close(client_fd);
        }
      }
    }
  }
}