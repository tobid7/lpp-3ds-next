#include <NDS.hpp>

#include <3ds.h>
#include <arpa/inet.h>
#include <citro3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RenderD7 Tasks impl

static std::vector<Thread> threads;

static void create(ThreadFunc entrypoint) {
  s32 prio = 0;
  svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
  Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 64 * 1024,
                               prio - 1, -2, false);
  threads.push_back(thread);
}

static void destroy(void) {
  for (u32 i = 0; i < threads.size(); i++) {
    threadJoin(threads.at(i), U64_MAX);
    threadFree(threads.at(i));
  }
}

//////////////////////////////////////////////////////////////////////

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool *thrd = nullptr;

#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000
static u32 *SOC_buffer = NULL;

const char *ip = "127.0.0.1";
int port = 8080;

int sockfd, new_sock;
int e;
struct sockaddr_in server_addr, new_addr;
socklen_t addr_size;

void nsocExit() {
  printf("[+]waiting for socExit...\n");
  thrd[0] = false;
  svcSleepThread(1000000000);
  socExit();
}

void nsocInit() {
  // allocate buffer for SOC service
  SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

  if (SOC_buffer == NULL) {
    printf("[-]failed to allocate\n");
  }
  Result ret;
  // Now intialise soc:u service
  if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    printf("[-]socInit: 0x%08X\n", (unsigned int)ret);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("[-]Error in socket");

    return;
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = gethostid();

  e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e < 0) {
    perror("[-]Error in bind");
  }
  printf("[+]Binding successfull.\n");
  char rip[52];
  u8 *addr = (u8 *)&server_addr.sin_addr.s_addr;
  sprintf(rip, "%hhu.%hhu.%hhu.%hhu", addr[0], addr[1], addr[2], addr[3]);
  printf("[+]Ip: %s Port: %d\n", rip, server_addr.sin_port);
  thrd = new bool;
  thrd[0] = true;
}

///////////////
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

///////////////

void nsocLoop() {
  // Set socket non blocking so we can still read input to exit
  fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
  if (int ret = listen(sockfd, 5) == 0) {
    printf("[+]Listening....\n");
  } else {
    printf("[-]Error in listening\n");
    printf("[-]code: %d %s\n", errno, strerror(errno));
    close(sockfd);
    close(e);
    sockfd = -1;
    e = -1;
    printf("[+]Force Quit Thread...\n");
  }

  while (true) {
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

    if (new_sock < 0) {
      if (errno != EAGAIN) {
        printf("[-]accept: %d %s\n", errno, strerror(errno));
        break;
      }
    } else {
      // set client socket to blocking to simplify sending data back
      fcntl(new_sock, F_SETFL, fcntl(new_sock, F_GETFL, 0) & ~O_NONBLOCK);
      printf("[+]Connecting port %d from %s\n", new_addr.sin_port,
             inet_ntoa(new_addr.sin_addr));
    }

    if (!thrd[0]) {
      printf("[+]Force Quit Thread...\n");
      close(sockfd);
      close(e);
      sockfd = -1;
      e = -1;
      break;
    }
    wellc wlc;
    int rl = recv(new_sock, &wlc, sizeof(wellc), 0);
    if (wlc.mgk != mgkwelc) {
      printf("[-]Broken Packet\n");
    }
    if (wlc.cmd == 1) {
      dbg_d dbg;
      dbg.mgk = mgkdbg;
      dbg.usg_cpu = C3D_GetProcessingTime();
      send(new_sock, &dbg, sizeof(dbg_d), 0);
    }
    svcSleepThread(1000000);
  }

  return;
}

namespace nds {
void Init() {
  nsocInit();
  create((ThreadFunc)nsocLoop);
}

void Exit() { destroy(); }
} // namespace nds