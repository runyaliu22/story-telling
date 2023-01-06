#include <stdlib.h>

#include <cerrno>
#include <iostream>

void check_page_number(const char * b, int errnoo, char * end) {
  //  std::cout << errnoo << std::endl;
  if (errnoo == ERANGE) {
    std::cout << "OFR!" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (*b == '\0' || *end != '\0') {
    std::cout << "Wrong!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main() {
  //std::cout << (unsigned long)(-1);

  std::string a = " 2222";

  char * end = NULL;

  size_t num = strtol(a.c_str(), &end, 10);

  check_page_number(a.c_str(), errno, end);

  std::cout << num << std::endl;

  //  std::cout << *b << std ::endl;

  //if (errno == ERANGE) {
  //std::cout << "OFR!" << std::endl;
  //}

  //  if (*b != '\0' && *end == '\0') {
  //std::cout << num << std::endl;
  //}
  //else {
  //std::cout << "Wrong!" << std::endl;
  // }

  //if (*b == '\0' || *end != '\0') {
  //std::cout << "Wrong!" << std::endl;
  // }
  //else {
  //std::cout << num << std::endl;
  //}
}
