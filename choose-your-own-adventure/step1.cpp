#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
  std::string a = argv[1];
  std::string b = a + "/story.txt";
  std::ifstream infile;
  infile.open(b);

  std::string s;
  std::vector<std::string> v;
  while (std::getline(infile, s)) {
    v.push_back(s);
    std::cout << s << std::endl;
  }
}