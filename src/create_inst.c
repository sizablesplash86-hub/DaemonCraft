// File 5

#include "config.h"

void crt_inst(const char *instance_name)
{
  char inst_path[128];
  snprintf(inst_path, sizeof(inst_path), "%s/%s", root_insts, instance_name);

  if (mkdir(inst_path, 0755) != 0) printf("Failed to create instance folder. May already exist.\n");

  else
  {
    printf("Creating %s instance...\n\n", instance_name);

    char edition[32];
    printf("Java or Bedrock: ");
    fgets(edition, sizeof(edition), stdin);
    edition[strcspn(edition, "\r\n")] = 0;

    if (strcmp(edition, "java") == 0)
    {
      printf("Creating Java instance...\n");

      char modded[32];
      char version[128];
      char min[10];
      char max[10];
      printf("Vanilla or modded (for mod loader like forge, fabric, quilt, neoforge, etc): ");
      fgets(modded, sizeof(modded), stdin);
      edition[strcspn(edition, "\r\n")] = 0;

      printf("Enter version number (eg: 26.3): ");
      fgets(version, sizeof(version), stdin);
      version[strcspn(version, "\r\n")] = 0;

      printf("Enter Minimum memory allocation (eg: 1  do not add 'G' at the end, leave blank): ");
      fgets(min, sizeof(min), stdin);

      printf("Enter maximum memory allocation (eg: 4  do not add 'G' at the end, leave blank): ");
      fgets(max, sizeof(max), stdin);
      
      char inst_conf_file[128];
      snprintf(inst_conf_file, sizeof(inst_conf_file), "%s/%s.conf", inst_path, instance_name);
      FILE *cf = fopen(inst_conf_file, "w");

      if (cf == NULL) printf("failed to create config file. Aborting...\n");

      else
      {
        fprintf(cf,
          "name=%s\n"
          "edition=%s\n"
          "version=%s\n"
          "modded=%s\n"
          "min=%s\n"
          "max=%s\n",
          instance_name, edition, version, modded, min, max
        );
        fclose(cf);

        printf("\nCreated config file\n\n");

        printf("Downloading %s jar...\n", version);
        char dwn[256];
        snprintf(dwn, sizeof(dwn), "wget %s/java/server_%s.jar -O %s/server_%s.jar", VRS_REPO, version, inst_path, version);
        if (system(dwn) != 0) printf("Failed to download JDS. Version number may be incorrect.\n");

        else
        {
          printf("Sucessfully downloaded JDS\n");
//          start_inst(instance_name);    // add this
        }
      }
    }  // end of Java

    if (strcmp(edition, "bedrock") == 0)
    {
      printf("Creating bedrock instance...\n");
      
      char version[128];
      printf("preview or latest: ");
      fgets(version, sizeof(version), stdin);
      version[strcspn(version, "\r\n")] = 0;

      char dwn[256];
      snprintf(dwn, sizeof(dwn), "wget %s/bedrock/%s.zip -O /tmp/%s.zip && unzip /tmp/%s.zip -d %s/ && rm /tmp/%s.zip", VRS_REPO, version, version, version, inst_path, version);
      if (system(dwn) != 0) printf("Failed to download BDS\n");

      else
      {
        printf("Successfully downloaded BDS!\n");
//          start_inst(instance_name);    // add this
      }
    }
  }  // end of the thingy
}