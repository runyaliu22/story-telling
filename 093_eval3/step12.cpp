#include <iostream>
#include <map>

int main() {
  std::map<char, int> mymap;

  // first insert function version (single parameter):
  mymap.insert(std::pair<char, int>('a', 100));
  mymap.insert(std::pair<char, int>('b', 200));

  std::map<char, int>::iterator it = mymap.begin();

  std::cout << "mymap contains:\n";
  for (it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}
