// File 7

#include <daemoncraft/core.h>

void create_minecraft_instance(const char *instance_name, const char *port, const char *edition, const char *version, const char *mod, const char *min, const char *max)
{
  char path[512];
  snprintf(path, sizeof(path), "/home/daemoncraft/instances/%s/", instance_name);
    
  if (mkdir(path, 0755) == 0)
  {
    printf("Successfully created instance folder: %s\n", path);
      
    // Write a local configuration file for this instance
    char conf_path[600];
    snprintf(conf_path, sizeof(conf_path), "%sinstance.conf", path);
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

      char eula[STR_LEN];
      snprintf(eula, sizeof(eula), "%seula.txt", path);
      FILE *e = fopen(eula, "w");
      if (e != NULL)
      {
        fprintf(f,
          "#By changing the setting below to TRUE you are indicating your agreement to our EULA (https://aka.ms/MinecraftEULA).\n"
          "#Sat Sep 12 16:14:56 CDT 2026\n"
          "eula=true\n"
        );
        fclose(e);
      }

      // remove this section cuz it's for testing
      char prop[1024];
      snprintf(prop, sizeof(prop), "%sserver.properties", path);
      FILE *p = fopen(prop, "w");
      if (e != NULL)
      {
        fprintf(f,
          "#Minecraft server properties\n"
          "#Sat Oct 03 13:57:14 CDT 2026\n"
          "accepts-transfers=false\n"
          "allow-flight=false\n"
          "broadcast-console-to-ops=true\n"
          "broadcast-rcon-to-ops=true\n"
          "bug-report-link=\n"
          "chat-spam-threshold-seconds=10\n"
          "command-spam-threshold-seconds=10\n"
          "difficulty=easy\n"
          "enable-code-of-conduct=false\n"
          "enable-jmx-monitoring=false\n"
          "enable-query=false\n"
          "enable-rcon=false\n"
          "enable-status=true\n"
          "enforce-secure-profile=true\n"
          "enforce-whitelist=false\n"
          "entity-broadcast-range-percentage=100\n"
          "force-gamemode=false\n"
          "function-permission-level=2\n"
          "gamemode=survival\n"
          "generate-structures=true\n"
          "generator-settings={}\n"
          "hardcore=false\n"
          "hide-online-players=false\n"
          "initial-disabled-packs=\n"
          "initial-enabled-packs=vanilla\n"
          "level-name=world\n"
          "level-seed=\n"
          "level-type=minecraft\\:normal\n"
          "log-ips=true\n"
          "management-server-allowed-origins=\n"
          "management-server-enabled=false\n"
          "management-server-host=localhost\n"
          "management-server-port=0\n"
          "management-server-secret=pqjW2SnKj6L816vUQl05y3zWYVu8fY3ARoHA1M4r\n"
          "management-server-tls-enabled=true\n"
          "management-server-tls-keystore=\n"
          "management-server-tls-keystore-password=\n"
          "max-chained-neighbor-updates=1000000\n"
          "max-players=20\n"
          "max-tick-time=60000\n"
          "max-world-size=29999984\n"
          "motd=A Minecraft Server\n"
          "network-compression-threshold=256\n"
          "online-mode=true\n"
          "op-permission-level=4\n"
          "pause-when-empty-seconds=60\n"
          "player-idle-timeout=0\n"
          "prevent-proxy-connections=false\n"
          "query.port=25569\n"
          "rate-limit=0\n"
          "rcon.password=\n"
          "rcon.port=25575\n"
          "region-file-compression=deflate\n"
          "require-resource-pack=false\n"
          "resource-pack=\n"
          "resource-pack-id=\n"
          "resource-pack-prompt=\n"
          "resource-pack-sha1=\n"
          "server-ip=\n"
          "server-port=25569\n"
          "simulation-distance=10\n"
          "spawn-protection=16\n"
          "status-heartbeat-interval=0\n"
          "sync-chunk-writes=true\n"
          "text-filtering-config=\n"
          "text-filtering-version=0\n"
          "use-native-transport=true\n"
          "view-distance=10\n"
          "white-list=false\n"
        );
        fclose(p);
      }
      //
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
      snprintf(jar, sizeof(jar), "wget %sjava/server_%s.jar -O %sserver_%s.jar", VRS_URL, version, path, version);
    //  printf("%s\n", jar);   // remove
      if (system(jar) != 0) printf("Failed to download JDS\n");
      else printf("JDS sucessfully downloaded to %s!\n", instance_name);
    }
    if (strcmp(edition, "bedrock") == 0)
    {
      printf("Downloading BDS...\n");
      char bed[STR_LEN];
      snprintf(bed, sizeof(bed), "wget %sbedrock/bedrock-server-latest.zip -O %sbedrock-server-latest.zip", VRS_URL, path);
    //  printf("%s\n", bed);   // remove
      if (system(bed) != 0) printf("Failed to download BDS\n");
      else printf("BDS sucessfully downloaded to %s!\n", instance_name);
      
      char zip[STR_LEN];
      snprintf(zip, sizeof(zip), "unzip %sbedrock-server-latest.zip -d %s", path, path);
      if (system(zip) != 0) perror("failed to extract archive\n");
      else printf("Archive extracted!\n");
      char rm[64];
      snprintf(rm, sizeof(rm), "rm %sbedrock-server-latest.zip", path);
      system(rm);
    }
    start_inst(instance_name);
  }
  else perror("Failed to create instance directory (folder may already exist)");
  // leaving this for now
//  start_inst(instance_name, edition, path, min, max, version, conf_path);
}