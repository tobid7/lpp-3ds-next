#include <3ds.h>
#include <arpa/inet.h>
#include <citro3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <NDS.hpp>


// RenderD7 Tasks impl

static std::vector<Thread> threads;

void create(ThreadFunc entrypoint) {
  s32 prio = 0;
  svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
  Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 64 * 1024,
                               prio - 1, -2, false);
  threads.push_back(thread);
}

void destroy(void) {
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
}

///////////////
struct wellc {
  uint32_t mgk;
  int cmd;
};
uint32_t mgkwelc = 0x54d75145;

struct dbg_d {
  uint32_t mgk;
  float usg_cpu;  // Usage in MS
  float usg_gpu;  // Usage in MS
  float usg_buf;  // Buf Usage
  int usg_mem;    // Mem in Bytes
  int usg_lmem;   // Lin in Bytes
};
uint32_t mgkdbg = 0x54d75a45;

///////////////

void nsocLoop() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Could not create socket" << std::endl;
    return;
  }

  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = gethostid();
  address.sin_port = htons(port);

  if (bind(server_socket, (sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Bind failed" << std::endl;
    return;
  }

  // Set socket non blocking so we can still read input to exit
  fcntl(server_socket, F_SETFL, fcntl(server_socket, F_GETFL, 0) | O_NONBLOCK);

  if (listen(server_socket, 5) == 0) {
    printf("[+]Listening...\n");
  } else {
    std::cerr << "[-]Listen failed" << std::endl;
    return;
  }

  sockaddr_in client_address;
  u32 clilen = sizeof(client_address);
  int client_socket = -1;

  thrd[0] = true;
  while (thrd[0]) {
    client_socket = accept(server_socket, (sockaddr *)&client_address, &clilen);
    if (client_socket < 0) {
      std::cerr << "[-]Accept failed: " << strerror(errno) << std::endl;
      return;
    } else {  // set client socket to blocking to simplify sending data back
      fcntl(client_socket, F_SETFL,
            fcntl(client_socket, F_GETFL, 0) & ~O_NONBLOCK);
      printf("[+]Connected to Client!\n");
    }
    // Dummy-Daten senden
    dbg_d data;
    data.mgk = mgkdbg;
    data.usg_cpu = C3D_GetProcessingTime();
    send(client_socket, &data, sizeof(data), 0);
    svcSleepThread(1000000000);
  }

  return;
}

namespace nds {
void Init() {
  nsocInit();
  create((ThreadFunc)nsocLoop);
}

void Exit() { destroy(); }
}  // namespace nds