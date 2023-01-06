#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
  if (argc == 1) {
    std::string s;
    std::vector<std::string> v;

    while (std::getline(std::cin, s)) {
      v.push_back(s);
    }

    if (!std::cin.eof()) {
      perror("Failure in file");
      return EXIT_FAILURE;
    }

    std::sort(v.begin(), v.end());
    for (size_t i = 0; i < v.size(); i++) {
      std::cout << v[i] << std::endl;
    }
  }

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::ifstream infile;
      infile.open(argv[i]);

      if (!infile.good()) {
        std::cerr << "Fail to open file!";
        return EXIT_FAILURE;
      }

      std::string s;
      std::vector<std::string> v;

      while (std::getline(infile, s)) {
        v.push_back(s);
      }

      if (!infile.eof()) {
        perror("Failure in file");
        return EXIT_FAILURE;
      }

      std::sort(v.begin(), v.end());
      for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << "\n";
      }
    }
  }
  return EXIT_SUCCESS;
}
