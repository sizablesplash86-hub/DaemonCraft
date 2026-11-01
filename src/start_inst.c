// File 6

#include "config.h"

void start_inst(const char *instance_name)
{
  char inst_path[128];
  snprintf(inst_path, sizeof(inst_path), "%s/%s", root_insts, instance_name);
  
  TODO: // add the file opening code here
  printf("Opening config...\n");

  char inst_conf[64];
  snprintf(inst_conf, sizeof(inst_conf), "%s/%s.conf", inst_path, instance_name);
  FILE *fp = fopen(inst_conf, "r");
  if (fp == NULL)
  {
    perror("Failed to open config file");
    return;
  }

  char line[1024];
  int found = 0;
  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if (strncmp(line, "version=", 8) == 0)
    {
      found = 1;
      break;
    }
  }
  fclose(fp);

  char *version = line + 8;
  if (found)
  {
    line[strcspn(line, ";\r\n")] = 0;
    printf("Instance version is %s...\n", version);
  }

  else
  {
    printf("Port not found\n");
    return;
  }
  
  printf("starting %s...\n", instance_name);

//  return;  // remove this


  int pipe_stdout[2]; // Child writes, parent reads (Logs)
  int pipe_stdin[2];  // Parent writes, child reads (Commands)

  pipe(pipe_stdout);
  pipe(pipe_stdin);

  pid_t pid = fork();
  if (pid == 0)
  {
    // --- CHILD PROCESS ---
    
    // Redirect stdout and stderr to the write end of the stdout pipe
    dup2(pipe_stdout[1], STDOUT_FILENO);
    dup2(pipe_stdout[1], STDERR_FILENO);
    
    // Redirect stdin to the read end of the stdin pipe
    dup2(pipe_stdin[0], STDIN_FILENO);
    
    // Close unused file descriptors in the child
    close(pipe_stdout[0]);
    close(pipe_stdin[1]);
    
    char jar_arg[128];
    snprintf(jar_arg, sizeof(jar_arg), "%s/server_%s.jar", inst_path, version);
    chdir(inst_path);
    execlp("java", "java", "-jar", jar_arg, "nogui", (char *)NULL);
    exit(1);
  }
  else
  {
    // --- PARENT PROCESS (DaemonCraft) ---
    
    // Close unused file descriptors in the parent
    close(pipe_stdout[1]);
    close(pipe_stdin[0]);
    
    // pipe_stdout[0] can now be added to your select() or poll() loop 
    // alongside your main network socket to stream server logs in real time.
    // Writing commands like "say Hello\n" to pipe_stdin[1] injects them into the server.

    printf("Created instance with PID %d\n", pid);

    FILE *cf = fopen(inst_conf, "a");
    if (cf != NULL)
    {
      fprintf(cf, "pid=%d", pid);
      fclose(cf);
    }
  }
}