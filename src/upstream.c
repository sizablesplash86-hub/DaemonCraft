// File 4

#include "config.h"
#include <sys/select.h>
#include <unistd.h>

void upstream(int sockfd)
{
  printf("Running loop...\n\nRunning interactive console! Enter 'list' to see a list of commands\n");
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

    if (FD_ISSET(STDIN_FILENO, &read_fds))  // FD_ISSET sys/select.h C.63 p.1332  // STDIN_FILENO unistd.h C.4 I think
    {
      if (fgets(cmd, sizeof(cmd), stdin) != NULL)
      {
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strncmp(cmd, "start ", 6) == 0)    // starts instance
        {
          char *instance_name = cmd + 6;
          char inst_path[64];
          char ls[128];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", root_insts, instance_name);
          snprintf(ls, sizeof(ls), "ls %s/ >/dev/null 2>&1", inst_path);
          if (system(ls) != 0)
          {
            printf("Instance \"%s\" not found!\n", instance_name);
            continue;
          }
          start_inst(instance_name);
        }

        else if (strncmp(cmd, "stop ", 5) == 0)    // stops instance
        {
          char *instance_name = cmd + 5;
          char inst_path[64];
          char ls[128];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", root_insts, instance_name);
          snprintf(ls, sizeof(ls), "ls %s/ >/dev/null 2>&1", inst_path);
          if (system(ls) != 0)
          {
            printf("Instance \"%s\" not found!\n", instance_name);
            continue;
          }
          printf("stopping %s...\n", instance_name);
//          stop_inst(inst_name);  // create this
        }

        else if (strncmp(cmd, "create ", 7) == 0)
        {
          char *instance_name = cmd + 7;
          char inst_path[64];
          char ls[128];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", root_insts, instance_name);
          snprintf(ls, sizeof(ls), "ls %s/ >/dev/null 2>&1", inst_path);

          if (system(ls) == 0)
          {
            printf("%s already exists\n", instance_name);
            continue;
          }
          crt_inst(instance_name);
        }

        else if (strcmp(cmd, "list") == 0)    // lists commands
        {
          printf(
            "\n[*] start [instance_name]\n"
            "  - starts an instance\n"
            "[*] stop [instance_name]\n"
            "  - stops an instance\n"
            "[*] create [instance_name]\n"
            "  - creates an instance\n"
            "[*] screen [instance_name]\n"
            "  - opens up instance console\n"
            "[*] ls instances\n"
            "  - lists instances and their active states\n"
            "[*] list\n"
            "  - lists commands\n"
            "[*] exit\n"
            "  - shuts down DaemonCraft\n\n"
          );
        }

        else if (strcmp(cmd, "exit") == 0)    // shuts down
        {
          printf("Stopping DaemonCraft upstream...\n");
          return;
        }
        
        else if (strlen(cmd) > 0) printf("\"%s\" not a valid command! Please try again\n", cmd);
      }  // end of reading cmd
      printf(">>> ");
      fflush(stdout);
    }  // end of reading user input


    if (FD_ISSET(sockfd, &read_fds))
    {
      int client_fd = accept(sockfd, NULL, NULL);
      if (client_fd >= 0)
      {
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
        else write(client_fd, NOT_FOUND, strlen(NOT_FOUND));

        fclose(fts);
        close(client_fd);
      }
    }
  } // end of loop
}