#include "config.h"

int crt_inst(const char *inst_name, const char *inst_path)
{
  printf("Creating %s...\n\n", inst_name);

  char edition[64], vers[256], mod[128], min[64], max[64];

  printf("Java or Bedrock: ");
  fgets(edition, sizeof(edition), stdin);
  edition[strcspn(edition, "\r\n")] = 0;

  if (strcmp(edition, "java") == 0 || strcmp(edition, "Java") == 0)
  {
    printf("Enter Java version: ");
    fgets(vers, sizeof(vers), stdin);
    vers[strcspn(vers, "\r\n")] = 0;

    printf("Vanilla or modded (enter 'list' to see mod loaders): ");
    fgets(mod, sizeof(mod), stdin);
    mod[strcspn(mod, "\r\n")] = 0;

    if (strcmp(mod, "list") == 0) 
    {
      printf("\n  vanilla\n  forge\n  fabric\n  forge\n  neoforge\n  quilt\n\n"); // check for others
      TODO: // add some kinda loop
    }

    printf("Enter minimum ram allocation (MB): ");
    fgets(min, sizeof(min), stdin);
    min[strcspn(min, "\r\n")] = 0;

    printf("Enter maximum ram allocation (MB): ");
    fgets(max, sizeof(max), stdin);
    max[strcspn(max, "\r\n")] = 0;
  }

  if (strcmp(edition, "bedrock") == 0 || strcmp(edition, "Bedrock") == 0)
  {
    printf("latest or preview: ");
    fgets(vers, sizeof(vers), stdin);
    vers[strcspn(vers, "\r\n")] = 0;
  }

  printf("Press enter to create the instance ");
  getchar();

  if (mkdir(inst_path, 0755) != 0)
  {
    printf("Failed to create instance folder. May already exist.\n");
    return -1;
  }

  printf("[*] Created folder...\n");

  char inst_conf[256];
  snprintf(inst_conf, sizeof(inst_conf), "%s/%s.conf", inst_path, inst_name);
  FILE *cf = fopen(inst_conf, "w");
  printf("[*] Creating config file...\n");
  if (cf == NULL)
  {
    printf("[*] Failed to create config file\n");
    return 1;
  }

  if (strcmp(edition, "java") == 0 || strcmp(edition, "Java") == 0)
  {
    fprintf(cf, 
      "edition %s\n"
      "version %s\n"
      "mod %s\n"
      "min %sM\n"
      "max %sM\n",
      edition, vers, mod, min, max
    );

    fclose(cf);
  }

  if (strcmp(edition, "bedrock") == 0 || strcmp(edition, "Bedrock") == 0)
  {
    fprintf(cf,
      "edition %s\n"
      "version %s\n",
      edition, vers
    );

    fclose(cf);
  }

  printf("[*] Created config file...\n");

  printf("[*] Downloading server...\n");
  if (dwnld_srvr(inst_path, vers, edition) != 0)
  {
    printf("Error occured! could not download server\n");
    return 1;
  }

  printf("[*] Downloaded server!\n");
  printf("[*] Instance created!\n");
  printf("Would you like to start %s now? (y/n): ", inst_name);
  char srt;
  scanf("%c", &srt);

  if (srt == 'y')
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (strcmp(edition, "java") == 0 || strcmp(edition, "Java") == 0)
    {
      printf("By pressing enter you agree to the Mojang End User License Agreement (EULA) https://www.minecraft.net/en-us/eula\nType 'q' if you do not agree or press enter to continue: ");
      char cho;
      scanf("%c", &cho);
      if (cho == 'q')
      {
        printf("If you desire to sign the EULA later, run 'eula [inst_name]' in the terminal\n");
        return 0;
      }
      else
      {
        printf("[*] Agreed to EULA\n");
        char eula[512];
        snprintf(eula, sizeof(eula), "%s/eula.txt", inst_path);
        FILE *el = fopen(eula, "w");
        if (el != NULL)
        {
          fprintf(el, "eula=true\n");
          fclose(el);
        }
        else
        {
          printf("Failed to create EULA\n");
          return -1;
        }
      }
    }

    inst_conf_file(inst_name, inst_path);
  }

  return 0;
}