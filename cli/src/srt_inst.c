#include "config.h"

int srt_inst(const char *inst_name, const char *inst_path, const char *inst_edition, const char *inst_version, const char *inst_mod, const char *inst_min, const char *inst_max)
{
  char log_path[256], fifo_path[256], jar_path[256], pid_file[256];
  
  printf("starting %s...\n", inst_name);

  int pipe_stdout[2]; // Child writes, parent reads (Logs)
  int pipe_stdin[2];  // Parent writes, child reads (Commands)

  pipe(pipe_stdout);
  pipe(pipe_stdin);

  pid_t pid = fork();
  
  if (pid == 0)
  {
    setsid();  // Makes it seperate. Want for DaemonCraft (right now), don't want for SpyderFly.
    
    // Redirect stdout and stderr to the write end of the stdout pipe
    dup2(pipe_stdout[1], STDOUT_FILENO);
    dup2(pipe_stdout[1], STDERR_FILENO);
    
    // Redirect stdin to the read end of the stdin pipe
    dup2(pipe_stdin[0], STDIN_FILENO);
    
    // Close unused file descriptors in the child
    close(pipe_stdout[0]);
    close(pipe_stdin[1]);

    chdir(inst_path);
    if (strcmp(inst_edition, "java") == 0 || strcmp(inst_edition, "Java") == 0)
    {
      char xms_arg[128], xmx_arg[128], jar_arg[512];
      snprintf(jar_arg, sizeof(jar_arg), "%s/server.jar", inst_path);
      snprintf(xms_arg, sizeof(xms_arg), "-Xms%s", inst_min);
      snprintf(xmx_arg, sizeof(xmx_arg), "-Xmx%s", inst_max);
      printf(
        "Jar path: %s\n"
        "Min ram: %s\n"
        "Max ram: %s\n",
        jar_arg, xms_arg, xmx_arg
      );

      if (execlp("java", "java", xms_arg, xmx_arg, "-jar", jar_arg, "nogui", NULL) != 0)
      {
        fprintf(stdout, "Failed to start Instance\n");
        return -1;
      }
      else fprintf(stdout, "Started instance\n");
    }
    if (strcmp(inst_edition, "bedrock") == 0 || strcmp(inst_edition, "Bedrock") == 0)
    {
      if (execlp("LD_LIBRARY_PATH=. ", "./bedrock_server", NULL) != 0)
      {
        fprintf(stderr, "Failed to start %s\n", inst_name);
        return -1;
      }
      else fprintf(stderr, "Started %s\n", inst_name);
    }
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