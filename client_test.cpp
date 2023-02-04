#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

/////////////////////
struct wellc {
  uint32_t mgk;
  int cmd;
};
uint32_t mgkwelc = 0x54d75145;

struct dbg_d {
  uint32_t mgk;
  float usg_cpu; // Usage in MS
  float usg_gpu; // Usage in MS
  float usg_buf; // Buf Usage
  int usg_mem;   // Mem in Bytes
  int usg_lmem;  // Lin in Bytes
};
uint32_t mgkdbg = 0x54d75a45;

/////////////////////

void DataOut(dbg_d dbg) {
  std::cout << "| CPU: " << dbg.usg_cpu << std::endl;
  std::cout.flush();
}

int main() {
  const char *ip = "locallhost";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Connected to Server.\n");

  while (true) {
    wellc wlc;
    wlc.mgk = mgkwelc;
    wlc.cmd = 1;
    send(sockfd, &wlc, sizeof(wellc), 0);
    usleep(1000000);
    dbg_d ret;
    recv(sockfd, &ret, sizeof(dbg_d), 0);
    if (ret.mgk != mgkdbg)
      break;
    DataOut(ret);
    usleep(1000000);
  }

  printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}