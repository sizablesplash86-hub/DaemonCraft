// File 8

#include <daemoncraft/core.h>

void start_inst(const char *instance_name)
{
  char path[STR_LEN];
  char conf_path[600];
  snprintf(path, sizeof(path), "/home/daemoncraft/instances/%s/", instance_name);
  snprintf(conf_path, sizeof(conf_path), "%sinstance.conf", path);

  char min_ram[32];
  char max_ram[32];
  char version[64];

  FILE *fp = fopen(conf_path, "r");
  if (fp != NULL)
  {
    char line[512];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      line[strcspn(line, "\r\n")] = 0;

      char temp[64];
      if (sscanf(line, "memory_min=%63s", temp) == 1)
      {
        temp[strcspn(temp, "G")] = 0;
        snprintf(min_ram, sizeof(min_ram), "%s", temp);
        printf("Xms found: %sGB\n", min_ram);
      }
      
      if (sscanf(line, "memory_max=%63s", temp) == 1)
      {
        temp[strcspn(temp, "G")] = 0;
        snprintf(max_ram, sizeof(max_ram), "%s", temp);
        printf("Xmx found: %sGB\n", max_ram);
      }

      if (sscanf(line, "version=%63s", temp) == 1)
      {
//        temp[strcspn(temp, "\r\n")] = 0;
        snprintf(version, sizeof(version), "%s", temp);
        printf("Version: %s\n", version);
      }
    }
    fclose(fp);
  }
  else
  {
    perror("Could not open instance.conf for reading");
  }


  pid_t pid = fork();
  if (pid < 0)
  {
    perror("Fork failed!\n");
    return;
  }

  if (pid == 0)
  {
    setsid();

    if (chdir(path) != 0)
    {
      perror("Failed to locate instance\n");
      exit(1);
    }
    
//    freopen(">/dev/null 2>&1", "r", stdin);  // No input expected from console
    
    freopen("console.log", "w", stdout); 
    freopen("console.log", "w", stderr);


    printf("Starting %s...\n", instance_name);
    char xms_arg[32], xmx_arg[32];
    snprintf(xms_arg, sizeof(xms_arg), "-Xms%sG", min_ram);
    snprintf(xmx_arg, sizeof(xmx_arg), "-Xmx%sG", max_ram);
    char jar_arg[256];
    snprintf(jar_arg, sizeof(jar_arg), "server_%s.jar", version);

    // Optional: Redirect stdout/stderr to a log file inside the instance directory 
    // so the server doesn't spam your terminal or get blocked on output pipes.
    // FILE *log = freopen("server.log", "w", stdout);
    // freopen("server.log", "w", stderr);

    execlp("java", "java", xms_arg, xmx_arg, "-jar", jar_arg, "nogui", NULL);
    perror("Failed to start instance\n");
    exit(1);


/*
    if (strcmp(edition, "bedrock") == 0)   // this won't start
    {
      printf("Starting %s...\n", instance_name);
    
      printf("not setup yet\n");

      if (execlp(path, "LD_LIBRARY_PATH=.", "./bedrock_server") != 0)
      {
        perror("failed to start instance\n");
      }
    }  */
    exit(0);
  } 
  else
  {
    printf("Spawned Minecraft instance background process with PID: %d\n", pid);
    char conf_path[600];
    snprintf(conf_path, sizeof(conf_path), "%sinstance.conf", path);
    
    FILE *f = fopen(conf_path, "a");
    if (f != NULL)
    {
      fprintf(f, "pid=%d\nstatus=running\n", pid);   // fix the bugs here
      fclose(f);
      printf("Instance PID saved to configuration.\n");
    }
    else perror("Failed to update instance.conf with PID");
  }
}