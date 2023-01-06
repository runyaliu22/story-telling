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
  while (std::getline(infile, s)) {
    if (s.find("page") != std::string::npos) {
      v.push_back(s.substr(0, 1));
      std::cout << v[v.size() - 1] << std::endl;
    }
  }
}
