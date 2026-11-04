#include "config.h"
#include <linux/limits.h>

int mng_insts(void)
{
  printf("\nWelcome to %s CLI manager!\nRun 'help' to see a list of commands\n\n", NAME);
  while (1)
  {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      printf(">>> ");
      if (fgets(cmd, sizeof(cmd), stdin) != NULL)
      {
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strcmp(cmd, "help") == 0)
        {
          printf("\n"
            "Commands:\n\n"
            
            "  'resume [instance_name]'    resume's an instance's console\n"
            "  'stop [instance_name]'      stops an instance\n"
            "  'start [instance_name]'     starts an instance\n"
            "  'create [instance_name]'    create a new instance\n"
            "  'delete [instance_name]'    deletes an entire instance\n"
            "  'eula [instance_name]'      signs the EULA for an instance\n\n"

            "  'nuke'        kills all active instances\n"
            "  'big-bang'    starts all instances\n"
            "  'exit'        exits this console\n\n"
          );
          continue;
        }

        else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
        {
          printf("Exiting current terminal...\n");
          break;
        }

        else if (strncmp(cmd, "create ", 7) == 0)
        {
          char *inst_name = cmd + 7;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);

          struct stat st;
          if (stat(inst_path, &st) == 0 && S_ISDIR(st.st_mode))  // might not need this
          {
            printf("%s already exists\n", inst_name);
            continue;
          }
          crt_inst(inst_name, inst_path);
        }

        else if (strncmp(cmd, "start ", 6) == 0)
        {
          char *inst_name = cmd + 6;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
          struct stat st;
          if (stat(inst_path, &st) != 0)
          {
            printf("%s does not exist\n", inst_name);
            continue;
          }
//          srt_inst(inst_name);
          inst_conf_file(inst_name, inst_path);
        }

        else if (strncmp(cmd, "stop ", 5) == 0)
        {
          char *inst_name = cmd + 5;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
          struct stat st;
          if (stat(inst_path, &st) != 0)
          {
            printf("%s does not exist\n", inst_name);
            continue;
          }
          if (stop_inst(inst_name) != 0)
          {
            printf("Failed to stop %s\n", inst_name);
            continue;
          }
        }

        else if (strncmp(cmd, "delete ", 7) == 0 || strncmp(cmd, "remove ", 7) == 0)
        {
          char *inst_name = cmd + 7;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
          struct stat st;
          if (stat(inst_path, &st) != 0)
          {
            printf("%s does not exist\n", inst_name);
            continue;
          }
          printf("Are you sure you want to delete %s? (y/n): ", inst_path);
          char cho;
          scanf("%c", &cho);
          if (cho == 'y')
          {
            char rm[256];
            snprintf(rm, sizeof(rm), "sudo rm -r %s", inst_path);
            if (system(rm) != 0)
//            if (rmdir(inst_path) != 0)
            {
              printf("Failed to delete %s\n", inst_name);
              continue;
            }
            else printf("Sucessfully deleted %s\n", inst_name);
          }
          else printf("Not deleting %s\n", inst_name);
        }

        else if (strncmp(cmd, "resume ", 7) == 0)
        {
          char *inst_name = cmd + 7;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
          struct stat st;
          if (stat(inst_path, &st) != 0)
          {
            printf("%s does not exist\n", inst_name);
            continue;
          }

          resume_inst(inst_name, inst_path);
        }

        else if (strcmp(cmd, "nuke") == 0)
        {
          printf("Are you sure you want to kill all instances? (y/n): ");
          TODO: // Add more
        }

        else if (strcmp(cmd, "big-bang") == 0)
        {
          printf("Would you like to start all instances? (y/n): ");
          TODO2: // add the stuff to start all
        }

        else if (strncmp(cmd, "eula ", 5) == 0)
        {
          char *inst_name = cmd + 5;
          char inst_path[256];
          snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
          struct stat st;
          if (stat(inst_path, &st) != 0)
          {
            printf("%s does not exist\n", inst_name);
            continue;
          }
          printf("By pressing enter, you agree to the Mojang End User License Agreement at https://www.minecraft.net/en-us/eula\nEnter 'q' to exit or enter to agree ");
          char agr;
          scanf("%c", &agr);
          if (agr == 'q')
          {
            printf("Eula not signed. Exiting now...\n");
            continue;
          }
          else
          {
            char eula[512];
            snprintf(eula, sizeof(eula), "%s/eula.txt", inst_path);
            FILE *eul = fopen(eula, "w");
            printf("[*] Signing EULA...\n");
            if (eul == NULL)
            {
              printf("Failed to create EULA\n");
              continue;
            }
            else
            {
              fprintf(eul, "eula=true\n");
              fclose(eul);

              printf("[*] Wrote EULA. Would you like to start %s? (y/n): ", inst_name);
              int c;
              while ((c = getchar()) != '\n' && c != EOF);
              
              char srt;
              scanf("%c", &srt);
              if (srt == 'y') inst_conf_file(inst_name, inst_path);
              if (srt == 'n')
              {
                printf("Not starting %s\n", inst_name);
                continue;
              }
            }
          }
        }

        else if (strcmp(cmd, "") == 0) continue;

        else printf ("Error! '%s' not a valid command\n", cmd);
      }
    }
  }
  return 0;
}