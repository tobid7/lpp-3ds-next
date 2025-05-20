#ifndef FTP_H
#define FTP_H
#include <3ds.h>

extern FS_Archive sdmcArchive;
extern char currentPath[];
extern u32 currentIP;
extern int dataPort;
extern char shared_ftp[256];

void ftp_init();
void ftp_exit();

int ftp_frame(int s);
int ftp_getConnection();

int ftp_openCommandChannel();
int ftp_openDataChannel();
int ftp_sendResponse(int s, int n, const char *mes);

#endif