#include "config.h"

int stop_inst(const char *inst_name)
{
  char inst_path[256], pid_file[256];
  snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
  snprintf(pid_file, sizeof(pid_file), "%s/%s.pid", inst_path, inst_name);

  FILE *pd = fopen(pid_file, "r");
  if (pd == NULL)
  {
    printf("Failed to open PID file\n");
    return -1;
  }

  char pid[32];
  fgets(pid, sizeof(pid), pd);

  fclose(pd);

  pid[strcspn(pid, "\r\n")] = 0;

  printf("PID = %s\n", pid);

  int pid_num = atoi(pid);

  if (kill(pid_num, SIGTERM) != 0) 
  {
    printf("Failed to stop %s\n", inst_name);
    return -1;
  }

  else printf("Successfully stopped %s\n", inst_name);

  if (remove(pid_file) != 0) printf("Failed to remove PID file\n");

  return 0;
}