#include <daemoncraft/core.h>

void ip_bind(void)
{
  int port_num = atoi(conf_port);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
  {
    perror("Socket creation failed");
    return;
  }

  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_num);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("Failed to bind address");
    return;
  }

  if (listen(sockfd, 10) < 0)
  {
    perror("Listen failed");
    return;
  }

    // 2. Pass the active socket into your monolith loop!
    // change here when testing

  run_daemoncraft_monolith(sockfd);
//  web(sockfd);  // not needed anymore, leaving for reference

  close(sockfd);
  return;
}