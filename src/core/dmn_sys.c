#include "config.h"

int dmn_sys(void)
{
  printf("\nWarning! This is intended for the background service. You should not interact with this. You can if you are in root.\n\n");

  if (geteuid() != 0) return -1;

  char *sudo_user = getenv("SUDO_USER");
  uid_t real_uid = getuid();

  if ((sudo_user != NULL && strcmp(sudo_user, "root") != 0) || real_uid != 0) return -1;

  printf("Starting %s background upstream...\n", NAME);

  DIR *ste = opendir(INSTS_ROOT);

  if (ste == NULL)
  {
    printf("Failed to open instances directory\n");
    return -1;
  }

  struct dirent *entry;

  while ((entry = readdir(ste)) != NULL)
  {
    if (entry->d_name[0] == '.' || 
      strcmp(entry->d_name, "index.php") == 0 || 
      strcmp(entry->d_name, "index.html") == 0) 
      continue;

    char inst_name[128];
    snprintf(inst_name, sizeof(inst_name), "%s", entry->d_name);

    printf(
    "=============\n"
    "Checking '%s'\n"
    "=============\n", 
    inst_name
    );

    struct stat st;
    char inst_pid[256], inst_path[256];
    snprintf(inst_path, sizeof(inst_path), "%s/%s", INSTS_ROOT, inst_name);
    snprintf(inst_pid, sizeof(inst_pid), "%s/%s.pid", inst_path, inst_name);
    if (stat(inst_pid, &st) != 0)
    {
      printf("%s is not to be started up\n", inst_name);
      continue;
    }
    else 
    {
      printf("Starting %s\n...", inst_name);
      inst_conf_file(inst_name, inst_path);
    }
    sleep(30);
  }

  closedir(ste);

  return 0;
}