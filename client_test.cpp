#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

const char *ip = "localhost";
int port = 8080;

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

void clientThread() {
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    std::cerr << "[-]Could not create socket";
    return;
  }
  printf("[+]Server socket created successfully.\n");

  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_address.sin_addr);

  if (connect(client_socket, (sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    std::cerr << "[-]Connect failed" << std::endl;
    return;
  }

  printf("[+]Connected to Server.\n");

  dbg_d data;
  while (true) {
    int bytesReceived = recv(client_socket, &data, sizeof(dbg_d), 0);
    if (bytesReceived <= 0) {
      std::cerr << "[-]Receive failed" << std::endl;
      return;
    }

    DataOut(data);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main() {
  std::thread thrd(clientThread);

  thrd.join();

  return 0;
}