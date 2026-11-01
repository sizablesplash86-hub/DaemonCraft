#include "config.h"

int srt_inst(const char *inst_name)
{
  char inst_path[128];
  snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
  
  TODO: // add the file opening code here
  printf("Opening config...\n");

  char inst_conf[256], log_path[256], fifo_path[256], jar_path[256], pid_file[256];
  snprintf(inst_conf, sizeof(inst_conf), "%s/%s.conf", inst_path, inst_name);
  FILE *fp = fopen(inst_conf, "r");
  if (fp == NULL)
  {
    printf("Failed to open config file\n");
    return -1;
  }

  char line[1024];
  int found = 0;
  while(fgets(line, sizeof(line), fp) != NULL)
  {
    if (strncmp(line, "vers = ", 7) == 0)
    {
      found = 1;
      break;
    }
  }
  fclose(fp);

  char *version = line + 7;
  if (found)
  {
    line[strcspn(line, ";\r\n")] = 0;
    printf("Instance version is %s...\n", version);
  }

  else
  {
    printf("Version not found\n");
    return -1;
  }
  
  printf("starting %s...\n", inst_name);

//  return;  // remove this


  int pipe_stdout[2]; // Child writes, parent reads (Logs)
  int pipe_stdin[2];  // Parent writes, child reads (Commands)

  pipe(pipe_stdout);
  pipe(pipe_stdin);

  pid_t pid = fork();
  if (pid == 0)
  {
    setsid();

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

    snprintf(pid_file, sizeof(pid_file), "%s/%s.pid", inst_path, inst_name);
    FILE *cf = fopen(pid_file, "w");
    if (cf != NULL)
    {
      fprintf(cf, "%d", pid);
      fclose(cf);
    }
  }


/*
  char inst_path[256];
  snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);

  // Path to version, log, and fifo
  char inst_conf[256], log_path[256], fifo_path[256], jar_path[256], pid_file[256];
  snprintf(inst_conf, sizeof(inst_conf), "%s/%s.conf", inst_path, inst_name);
  snprintf(log_path, sizeof(log_path), "%s/server.log", inst_path);
  snprintf(fifo_path, sizeof(fifo_path), "%s/instance.stdin", inst_path);
  snprintf(pid_file, sizeof(pid_file), "%s/%s.pid", inst_path, inst_name);

  // 1. Create the Named Pipe (FIFO) if it doesn't already exist
  mkfifo(fifo_path, 0644);

  pid_t pid = fork();
  if (pid < 0)
  {
    perror("Fork failed");
    return 1;
  }

  if (pid == 0)
  {
    // --- CHILD PROCESS ---
    setsid();

    if (chdir(inst_path) != 0)
    {
      perror("Failed to locate instance path");
      exit(1);
    }

    // Redirect stdout & stderr to server.log
    int log_fd = open(log_path, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (log_fd >= 0)
    {
      dup2(log_fd, STDOUT_FILENO);
      dup2(log_fd, STDERR_FILENO);
      close(log_fd);
    }

    // Redirect stdin from our FIFO (O_RDONLY | O_NONBLOCK prevents hanging if nothing writes yet)
    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd >= 0)
    {
      dup2(fifo_fd, STDIN_FILENO);
      close(fifo_fd);
    }

    // TODO: Parse version from conf and set up jar_arg dynamically
    // For now, assume standard jar execution:
    execlp("java", "java", "-jar", "server_26.3.jar", "nogui", (char *)NULL);
    exit(1);
  }
  else
  {
    // --- PARENT PROCESS ---
    printf("Instance '%s' started in background with PID: %d\n", inst_name, pid);
    
    // Save PID to config
    FILE *cf = fopen(pid_file, "w");
    if (cf != NULL)
    {
      fprintf(cf, "%d", pid);
      fclose(cf);
    }
  }
*/
  return 0;
}