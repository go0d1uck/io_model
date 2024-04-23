

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>

#include "business_logic.hpp"
#include "thread_pool.hpp"

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cout << "Socket Error: " << std::strerror(errno);
    return -1;
  }
  struct sockaddr_in addr;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8888);
  addr.sin_family = AF_INET;
  if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    std::cout << "Bind Error: " << std::strerror(errno);
    return -1;
  }
  if (listen(server_fd, 5) < 0) {
    std::cout << "Listen Error: " << std::strerror(errno);
    return -1;
  }
  // build a thread pool
  ThreadPool server_thread_pool(20);  // create 20 thread
  int cnt = 0;
  for (;;) {
    std::cout << "Waiting for connect" << std::endl;
    int connected_fd = accept(server_fd, NULL, NULL);
    server_thread_pool.enqueue(business_logic_with_fd, connected_fd, cnt++, 5);
  }
}