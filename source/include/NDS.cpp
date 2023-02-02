#include <NDS.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <3ds.h>

// RenderD7 Tasks impl

static std::vector<Thread> threads;

static void create(ThreadFunc entrypoint)
{
    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 64 * 1024,
                                 prio - 1, -2, false);
    threads.push_back(thread);
}

static void destroy(void)
{
    for (u32 i = 0; i < threads.size(); i++)
    {
        threadJoin(threads.at(i), U64_MAX);
        threadFree(threads.at(i));
    }
}

//////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000
static u32 *SOC_buffer = NULL;

char *ip = "127.0.0.1";
int port = 8080;

int sockfd, new_sock;
int e;
struct sockaddr_in server_addr, new_addr;
socklen_t addr_size;

void nsocExit()
{
    printf("[+]waiting for socExit...\n");
    close(sockfd);
    socExit();
}

void nsocInit()
{
    // allocate buffer for SOC service
    SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

    if (SOC_buffer == NULL)
    {
        printf("[-]failed to allocate\n");
    }
    Result ret;
    // Now intialise soc:u service
    if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0)
    {
        printf("[-]socInit: 0x%08X\n", (unsigned int)ret);
    }
    atexit(nsocExit);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Error in socket");

        return;
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = gethostid();

    e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0)
    {
        perror("[-]Error in bind");
    }
    printf("[+]Binding successfull.\n");
    char rip[52];
    u8 *addr = (u8 *)&server_addr.sin_addr.s_addr;
    sprintf(rip, "%hhu.%hhu.%hhu.%hhu", addr[0], addr[1], addr[2], addr[3]);
    printf("[+]Ip: %s Port: %d\n", rip, server_addr.sin_port);
}

void nsocLoop()
{
    nsocInit();

    // Set socket non blocking so we can still read input to exit
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);

    while (true)
    {
        if (listen(sockfd, 5) == 0)
        {
            printf("[+]Listening....\n");
        }
        else
        {
            perror("[-]Error in listening");

            destroy(); // Self destroy
        }

        addr_size = sizeof(new_addr);
        new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
        svcSleepThread(1000000000);
    }
}

namespace nds
{
    void Init()
    {
        create((ThreadFunc)nsocLoop);
    }

    void Exit()
    {
        destroy();
    }
}