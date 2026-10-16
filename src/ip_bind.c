// File 3

#include "config.h"

void ip_bind(void)
{
  int port_num = atoi(conf_port);  // provided by stdlib  converts char to int

  // 56.1 p.1150  // fd = socket(domain, type, protocol);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // sockfd is the integer made for this program    socket(); is provided by sys/socket C.56.2 p.1153  AF_NET is the ipv4, & SOCK_STREAM is... idk. Looked it up, I still don't know.
  if (sockfd < 0)  // creates the socket & makes sure it isn't less than 0... I think??
  {
    printf("Socket creation failed\n");
    return;
  }

  int optval;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));  // p.1278 c.61.9   I understand none of this

  struct sockaddr_in addr;  // p.1203 but it shows ipv6, this is ipv4
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port_num);

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)  // C.56.3 p.1153
  {
    printf("Failed to bind address\n");
    return;
  }

  if (listen(sockfd, 10) < 0)  // C.56.5.1 p.1156
  {
    printf("Failed to listen on port %s\n", conf_port);
    return;
  }

  upstream(sockfd);

  close(sockfd);  // requires unistd for some reason. C.56.5.5 p.1159
}