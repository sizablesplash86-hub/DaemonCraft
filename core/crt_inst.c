#include <daemoncraft/core.h>

void create_minecraft_instance(const char *instance_name, const char *port, const char *edition, const char *version, const char *mod, const char *min, const char *max)
{
  char path[512];
  snprintf(path, sizeof(path), "/home/daemoncraft/instances/%s", instance_name);
    
  if (mkdir(path, 0755) == 0)
  {
    printf("Successfully created instance folder: %s\n", path);
      
    // Write a local configuration file for this instance
    char conf_path[600];
    snprintf(conf_path, sizeof(conf_path), "%s/instance.conf", path);
  //  printf("Instance path is %s\n", path);   // remove
    if (strcmp(edition, "java") == 0)
    {
      printf("Creating Java config file...\n");
      FILE *f = fopen(conf_path, "w");
      if (f != NULL) 
      {
        fprintf(f,
          "name=%s\n"
          "port=%s\n"
          "edition=%s\n"
          "version=%s\n"
          "modded=%s\n"
          "memory_min=%sG\n"
          "memory_max=%sG\n"
          "status=idle\n",
          instance_name, port, edition, version, mod, min, max
        );
      fclose(f);
      }
    }
    if (strcmp(edition, "bedrock") == 0)
    {
      printf("Creating Bedrock config file...\n");
      FILE *f = fopen(conf_path, "w");
      if (f != NULL)
      {
        fprintf(f,
          "name=%s\n"
          "port=%s\n"
          "edition=%s\n"
          "version=bds_%s\n"
          "status=idle\n",
          instance_name, port, edition, version
        );
        fclose(f);
      }
    }

    printf("Instance configuration written successfully.\n");
      
    if (strcmp(edition, "java") == 0)
    {
      printf("Downloading JDS...\n");
      char jar[STR_LEN];
      snprintf(jar, sizeof(jar), "wget %sjava/server_%s.jar -O %s/server_%s.jar", VRS_URL, version, path, version);
    //  printf("%s\n", jar);   // remove
      if (system(jar) != 0) printf("Failed to download JDS\n");
      else printf("JDS sucessfully downloaded to %s!\n", instance_name);
    }
    if (strcmp(edition, "bedrock") == 0)
    {
      printf("Downloading BDS...\n");
      char bed[STR_LEN];
      snprintf(bed, sizeof(bed), "wget %sbedrock/bedrock-server-latest.zip -O %s/bedrock-server-latest.zip", VRS_URL, path);
    //  printf("%s\n", bed);   // remove
      if (system(bed) != 0) printf("Failed to download BDS\n");
      else printf("BDS sucessfully downloaded to %s!\n", instance_name);
    }

  }
  else perror("Failed to create instance directory (folder may already exist)");
}