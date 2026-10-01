#include "core.h"

void create_minecraft_instance(const char *name, int port, const char *edition, const char *version)
{
  char path[512];
  snprintf(path, sizeof(path), "/home/daemoncraft/instances/%s", name);
    
  // Create directory with standard permissions (rwxr-xr-x)
  if (mkdir(path, 0755) == 0)
  {
    printf("Successfully created instance folder: %s\n", path);
      
    // Write a local configuration file for this instance
    char conf_path[600];
    snprintf(conf_path, sizeof(conf_path), "%s/instance.conf", path);
    FILE *f = fopen(conf_path, "w");
    if (f != NULL) 
    {
      fprintf(f, "name=%s\n", name);
      fprintf(f, "port=%d\n", port);
      fprintf(f, "edition=%s\n", edition);
      fprintf(f, "version=%s\n", version);
      fprintf(f, "status=idle\n");
      fclose(f);
      printf("Instance configuration written successfully.\n");
    }
  }
  else perror("Failed to create instance directory (folder may already exist)");
}
