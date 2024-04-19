#include "business_logic.hpp"

#include <unistd.h>

#include <iostream>

void business_logic(unsigned int idx, unsigned int delay_second) {
  for (unsigned int i = 0; i < delay_second; i++) {
    std::cout << "I am " << idx << " process,"
              << "and now it's the " << i << " second of processing.\n";
    sleep(1);
  }
}

void business_logic_with_fd(int fd, unsigned int idx,
                            unsigned int delay_second) {
  for (unsigned int i = 0; i < delay_second; i++) {
    std::cout << "I am " << idx << " process,"
              << "and now it's the " << i << " second of processing.\n";
    sleep(1);
  }
  close(fd);
}