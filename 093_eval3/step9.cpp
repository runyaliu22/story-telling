#include <iostream>
#include <vector>

int main() {
  std::vector<std::pair<std::string, size_t> > jiayou;
  jiayou.push_back({"a", 3});
  jiayou.push_back({"a", 4});
  std::cout << jiayou[0].first << std::endl;
  std::cout << jiayou[1].first << std::endl;
}
