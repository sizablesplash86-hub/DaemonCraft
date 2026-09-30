#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

//             https://piston-meta.mojang.com/mc/game/version_manifest_v2.json         use this somehow
// see /home/mc-server/und-campus-build or whatever for references

char mc[16];

int main()
{
  if (system("command -v java > /dev/null 2>&1") != 0)
  {
    printf("\nJava Not installed\n\nInstalling now...");
    
    system("wget https://download.oracle.com/java/26/latest/jdk-26_linux-x64_bin.deb -O /tmp/java.deb");
    if (system("sudo apt install /tmp/java.deb") != 0)
    {
      printf("Java failed to install. Exiting now...");
      return 0;
    }
    system("rm /tmp/java.deb");
    if (system("java --version") != 0)
    {
      printf("Java failed to install. Exiting now...");
      return 0;
    }
    //continuing
  }

  if (system("ls -d /home/mc-server > /dev/null 2>&1") != 0)
  {
    system("mkdir /home/mc-server");
  }

  char mod;
  printf("Would you like to create modded server? y/n: ");
  scanf(" %c", &mod);
  if (mod == 'y')
  {
    printf("Not setup yet\n\n");
  }

  if (mod == 'y')
  {
    printf("Not setup yet\n\n");
  }
  
  printf("Enter desired Minecraft version: ");
  scanf(" %15s", mc);

  if (strcmp(mc, "26.2") == 0)
  {
    printf("not setup yet\n\n");
  }

  return 0;
}