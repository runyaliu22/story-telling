#include <fstream>   //file
#include <iostream>  //std
#include <vector>

int main(int argc, char ** argv) {
  std::string a = argv[1];
  std::string b = a + "/story.txt";
  std::ifstream infile;
  infile.open(b);

  std::string s;
  std::vector<std::string> v;
  std::vector<std::string> c;
  while (std::getline(infile, s)) {
    if (s.find("page") != std::string::npos) {
      v.push_back(s.substr(0, 1));
      //std::cout << v[v.size() - 1] << std::endl;
    }

    if (s.find(":") != s.find_last_of(":")) {
      size_t first = s.find(":");
      size_t last = s.find_last_of(":");
      size_t num = stoi(s.substr(0, first));
      //std::cout << num << std::endl;
      //std::string modified;
      //modified = s.substr(first + 1);

      c.push_back(s.substr(last + 1));
      std::cout << c[c.size() - 1] << std::endl;
    }
  }
}
