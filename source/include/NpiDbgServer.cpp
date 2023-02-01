#include <NpiDbgServer.hpp>
#include <ErrorHelper.hpp>
#include <sstream>
#include <3ds.h>
#include <citro3d.h>
#include <memory.h>
#include <vector>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

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
// Net impl (Luma Socket lol)
#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000

int sock, bytes_recieved;
struct hostent *host;
struct sockaddr_in server_addr;
static u32 *SOC_buffer = NULL;

//////////////////////////////////////////////////////////////////////

struct Command
{
    char magic[4];
    int cmd;
};

struct DbgData
{
    char magic[4];
    float usg_cpu;
    float usg_gpu;
    float usg_buf;
    float usg_ram;
    float usg_lrm;
    char nstdout[600];
};

void
MakeCommand(Command &cmd)
{
    cmd.magic[0] = 'C';
    cmd.magic[1] = 'M';
    cmd.magic[2] = 'D';
    cmd.magic[3] = '7';
}

bool CheckCommand(Command &cmd)
{
    if (cmd.magic[0] == 'C' && cmd.magic[1] == 'M' &&
        cmd.magic[2] == 'D' && cmd.magic[3] == '7')
        return true;
    return false;
}

void MakeDbgData(DbgData &cmd)
{
    cmd.magic[0] = 'D';
    cmd.magic[1] = 'B';
    cmd.magic[2] = 'G';
    cmd.magic[3] = 'D';
}

bool CheckDbgData(DbgData &cmd)
{
    if (cmd.magic[0] == 'D' && cmd.magic[1] == 'B' &&
        cmd.magic[2] == 'G' && cmd.magic[3] == 'D')
        return true;
    return false;
}
namespace Npi
{
    void InitDbgServer()
    {
        // allocate buffer for SOC service
        SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

        if (SOC_buffer == NULL)
        {
            ErrorHelper::WriteErr(__FILE__ + std::to_string(__LINE__));
            socExit();
            return;
        }
        int ret = 0;

        // Now intialise soc:u service
        if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0)
        {
            ErrorHelper::WriteErr(__FILE__ + std::to_string(__LINE__));
            socExit();
            return;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == -1)
        {
            ErrorHelper::WriteErr(__FILE__ + std::to_string(__LINE__));
            socExit();
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(6843);
        server_addr.sin_addr.s_addr = gethostid();

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            ErrorHelper::WriteErr(__FILE__ + std::to_string(__LINE__));
            socExit();
            return;
        }

        if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK) == -1)
        {
            ErrorHelper::WriteErr(__FILE__ + std::to_string(__LINE__));
            socExit();
            return;
        }

        create((ThreadFunc)DbgServerLoop);
    }

    void DbgServerLoop()
    {
        while (true)
        {
            Command cmd;
            int ret = recv(sock, &cmd, sizeof(Command), 0);
            if (CheckCommand(cmd))
            {
                if (cmd.cmd == 1)
                {
                    DbgData dbgd;
                    MakeDbgData(dbgd);
                    dbgd.usg_cpu = C3D_GetProcessingTime();
                    dbgd.usg_gpu = C3D_GetDrawingTime();
                    dbgd.usg_buf = C3D_GetCmdBufUsage();
                    dbgd.usg_lrm = linearSpaceFree();
                    dbgd.usg_ram = 0;
                    sprintf(dbgd.nstdout, "stdout not implemented yet!");
                    send(sock, &dbgd, sizeof(DbgData), 0);
                }
            }
        }
    }

    void ExitDbgServer()
    {
        destroy();
    }
}