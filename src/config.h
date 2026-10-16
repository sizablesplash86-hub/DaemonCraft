/* This is version 2.0 */


/* Notes:
 * If you ever want to replicate this true stream-redirection model in C without messing with screen's source code, you don't even need a PTY.
 * You can use standard Unix pipes pipe(), fork, redirect the child's file descriptors using dup2(), and then monitor the read-end of the pipe using select() or poll() alongside your web socket loops!
 */


/* Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* POSIX headers */
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>  // C.59.7 p.1207

/* Define */
#define DAEMONCRAFT_VRS "v0.2.0"
#define NOT_FOUND "HTTP/1.1 404 Not Found\r\n" "Content-Type: text/html; charset=UTF-8\r\n" "Connection: close\r\n\r\n" "<html><head><title>SpyderFly Site Not Found</title></head>" "<body><center><h1>SpyderFly Site Not Found</h1></center>" "<center>DaemonCraft is a branch of SpyderFly. Visit <a href=\"https://spyderfly.sizablesplash.com/support/\">https://spyderfly.sizablesplash.com/support/</a> for support</center></body></html>"
#define HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"

/* Characters */
extern char root[64];
extern char conf_file[128];
extern char conf_port[10];

void upstream_port(void);
void upstream(int sockfd);
void ip_bind(void);
void console(void);
