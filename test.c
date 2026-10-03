#include "include/core.h"

#define version "26.3"
#define path "/home/daemoncraft/instances"

int main()
{
  char jar[STR_LEN];
  char bed[STR_LEN];

  snprintf(jar, sizeof(jar), "wget %sserver_%s.jar -O %s/server_%s.jar", VRS_URL, version, path, version);
  printf("%s\n", jar);
  if (system(jar) != 0) printf("Failed to download JDS\n");
  else printf("JDS downloaded sucessfully!\n");

//  snprintf(bed, sizeof(bed), "wget %sbedrock-server-latest.zip -O %s/bedrock-server-latest.zip", VRS_URL, path);
//  if (system(bed) != 0) printf("Failed to download BDS\n");
//  else printf("BDS downloaded sucessfully!\n");
}

/*

root@deb:/mnt/code-projects/packages/minecraft# ./test
wget https://sizablesplash.com/server-guide/prereqs/OS-config/minecraft-server/versions/server_/home/daemoncraft/instances.jar -O UH��AWAVE1�AUL�-T/server_(null).jar
sh: 1: Syntax error: "(" unexpected
Failed to download JDS
root@deb:/mnt/code-projects/packages/minecraft

*/