#include <iostream>

int main() {
  while (true) {
    std::string s;
    std::getline(std::cin, s);
    size_t a = stoi(s);
    std::cout << a << std::endl;
  }
}
