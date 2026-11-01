#include "config.h"

int resume_inst(const char *inst_name, const char *inst_path)
{
  char log_path[256], fifo_path[256];
  snprintf(log_path, sizeof(log_path), "%s/server.log", inst_path);
  snprintf(fifo_path, sizeof(fifo_path), "%s/instance.stdin", inst_path);

  printf("\n--- Attaching to console for '%s' (Type 'detach' to exit) ---\n", inst_name);

  // 1. Open log file to read existing output
  FILE *log_fp = fopen(log_path, "r");
  if (log_fp != NULL)
  {
    char buffer[512];
    // Print existing log contents (simplified tail print)
    while (fgets(buffer, sizeof(buffer), log_fp) != NULL)
    {
      printf("%s", buffer);
    }
    // Keep log_fp open or reopen it to tail live output using select/poll, 
    // or use a simple loop.
  }

  // 2. Interactive input loop
  while (1)
  {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    // Optional: add log file descriptor to read_fds to stream live logs in real-time!

    struct timeval tv = {0, 100000}; // 100ms timeout
    int activity = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);

    if (activity > 0 && FD_ISSET(STDIN_FILENO, &read_fds))
    {
      char input_cmd[256];
      if (fgets(input_cmd, sizeof(input_cmd), stdin) != NULL)
      {
        input_cmd[strcspn(input_cmd, "\r\n")] = 0;

        if (strcmp(input_cmd, "detach") == 0)
        {
          printf("Detaching from console...\n");
          break;
        }

        // Write command into the FIFO so the Java process receives it on stdin
        int fifo_fd = open(fifo_path, O_WRONLY);
        if (fifo_fd >= 0)
        {
          write(fifo_fd, input_cmd, strlen(input_cmd));
          write(fifo_fd, "\n", 1);
          close(fifo_fd);
        }
        else
        {
          perror("Failed to write to instance input channel");
        }
      }
    }
  }

  if (log_fp) fclose(log_fp);
  return 0;
}