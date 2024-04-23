#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "business_logic.hpp"

int main() {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (listen_fd < 0) {
    std::cout << "Get listen_fd error!" << std::endl;
  }
  struct sockaddr_in serverAddr, clientAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(8888);
  if (bind(listen_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cout << "Bind Error!:" << strerror(errno) << std::endl;
    return 0;
  }
  listen(listen_fd, 2);
  while (true) {
    std::cout << "Waiting for client!\n";
    int client_fd = accept(listen_fd, nullptr, nullptr);
    if (client_fd < 0) {
      std::cout << "Accept error:" << strerror(errno) << std::endl;
    }
    business_logic(1, 5);
    close(client_fd);
  }
  return 0;
}