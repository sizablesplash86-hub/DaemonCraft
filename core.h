#define CORE_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/sockaddr.h>

#define FILE_SIZE 1024
#define STR_LEN 256

extern char web_path[STR_LEN];
extern char conf[STR_LEN];
extern char conf_port[STR_LEN];
extern char lan_ip[STR_LEN];

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
void web(void);
void ipv4(void);