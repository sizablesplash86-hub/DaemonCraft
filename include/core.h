// Anyone reading this, be aware how I made this work was that I did a symlink with the directory here to /usr/include/daemoncraft/ as that was the best way I could think of to get the headers seperate and not have code server think it's errors
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stddef.h>
#include <ifaddrs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/sockaddr.h>

#define FILE_SIZE 1024
#define STR_LEN 256
#define VER_JSON "https://launchermeta.mojang.com/mc/game/version_manifest_v2.json"
#define JAVA "25565"
#define BDRCK "19132"

#define FBRC_URL "https://meta.fabricmc.net/v2/versions/loader/26.3/0.19.5/1.1.2/server/jar"  // change the 26.3 part
#define VRS_URL "https://repo.sizablesplash.com/daemoncraft/versions/"  // add at the end for java or bedrock. ex:   %sjava/   %sbedrock/

extern char web_path[STR_LEN];
extern char conf[STR_LEN];
extern char conf_port[STR_LEN];
extern char lan_ip[STR_LEN];
extern char ipv4[STR_LEN];
extern char cmd[STR_LEN];

int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

typedef struct
{
  char domain[STR_LEN];
  char root[STR_LEN];
  int port;
} 
SiteConfig;

void ip(void);
void pubip(void);
void ip_bind(void);
void findport(void);
void run_daemoncraft_monolith(int sockfd);
void handle_client_request(int client_fd, SiteConfig *site, const char *request_buffer);
void create_minecraft_instance(const char *instance_name, const char *port, const char *edition, const char *version, const char *mod, const char *min, const char *max);