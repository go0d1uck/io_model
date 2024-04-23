#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstddef>
#include <cstring>
#include <iostream>
#include <thread>

#include "business_logic.cpp"

int main() {
  int i_server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (i_server_fd < 0) {
    std::cout << "Get server fd error: " << strerror(errno) << std::endl;
    return -1;
  }
  int opt = 1;
  setsockopt(i_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  struct sockaddr_in server_sockaddr;
  server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_sockaddr.sin_port = htons(8888);
  server_sockaddr.sin_family = AF_INET;
  if (bind(i_server_fd, (sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) <
      0) {
    std::cout << "Bind error: " << strerror(errno) << std::endl;
    return -1;
  }
  if (listen(i_server_fd, 2) < 0) {
    std::cout << "Listen error: " << strerror(errno) << std::endl;
    return -1;
  }
  int cnt = 1;
  for (;;) {
    std::cout << "Waiting for connect..\n";
    auto connected_fd = accept(i_server_fd, NULL, NULL);
    if (connected_fd < 0) {
      std::cout << "Accept error: " << std::strerror(errno) << std::endl;
      return -1;
    }
    std::thread(business_logic_with_fd, connected_fd, cnt++, 5).detach();
  }
  return 0;
}