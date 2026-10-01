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

extern char web_path[STR_LEN];
extern char conf[STR_LEN];
extern char conf_port[STR_LEN];
extern char lan_ip[STR_LEN];
extern char ipv4[STR_LEN];

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

void findport(void);
void ip(void);
void web(int sockfd);
void pubip(void);
void run_daemoncraft_monolith(int sockfd);
void ip_bind(void);
void create_minecraft_instance(const char *name, int port, const char *edition, const char *version);