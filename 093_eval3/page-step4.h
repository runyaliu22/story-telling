#include <algorithm>
#include <fstream>   //file
#include <iostream>  //std
#include <map>
#include <vector>

class page {
 private:
  //store relative information about choices of each page
  class choice {
   public:
    std::vector<std::string> choices;  //choices of each page
    std::vector<size_t> destinations;  //destinations of each page
    std::vector<std::pair<std::string, long int> > conditionmap;  //a map of conditions
  };

  choice * c;

  std::vector<std::pair<std::string, long int> > mymap;

  std::string dn;  //directory name
  std::string pn;  //page name
  char type;
  size_t condition_flag;

 public:
  page(std::string _dn, std::string _pn, char _type) :
      c(new choice()), dn(_dn), pn(_pn), type(_type), condition_flag(0) {}

  //deep copy constructor
  page(const page & rhs) :
      c(new choice(*(rhs.c))),
      dn(rhs.dn),
      pn(rhs.pn),
      type(rhs.type),
      condition_flag(rhs.condition_flag) {}

  //assignment operator
  page & operator=(const page & rhs) {
    if (this != &rhs) {
      dn = rhs.dn;
      pn = rhs.pn;
      type = rhs.type;
      condition_flag = rhs.condition_flag;
      delete c;
      c = new choice(*(rhs.c));
    }
    return *this;
  }

  ~page() { delete c; }

  char gettype() { return type; }

  choice * getchoice() { return c; }

  std::vector<std::pair<std::string, long int> > getmymap() { return mymap; }

  size_t get_flag() { return condition_flag; }

  void set_flag() { condition_flag = 1; }

  void addchoices(std::string addstring) { c->choices.push_back(addstring); }

  void adddestinations(size_t adddestination) {
    c->destinations.push_back(adddestination);
  }

  void addpair(std::string mystring, long int myvalue) {
    mymap.push_back(std::make_pair(mystring, myvalue));
  }

  void addconditions(std::string mystring, long int myvalue) {
    c->conditionmap.push_back(std::make_pair(mystring, myvalue));
  }

  void printpage_unavailable(std::vector<int> unavailable_index) {
    std::ifstream file;

    file.open((dn + pn).c_str());

    std::string s;

    while (getline(file, s)) {
      std::cout << s << std::endl;
    }
    std::cout << std::endl;

    std::cout << "What would you like to do? " << std::endl;
    std::cout << std::endl;
    std::vector<int>::iterator it;

    for (size_t i = 0; i < c->choices.size(); i++) {
      it = find(unavailable_index.begin(), unavailable_index.end(), i);

      if (it != unavailable_index.end()) {
        std::cout << " " << i + 1 << ". "
                  << "<UNAVAILABLE>" << std::endl;
        continue;
      }

      std::cout << " " << i + 1 << ". " << c->choices[i] << std::endl;
    }
  }

  void printpage() {
    std::ifstream file;

    //directory of the page

    file.open((dn + pn).c_str());

    std::string s;

    while (getline(file, s)) {
      std::cout << s << std::endl;
    }
    std::cout << std::endl;

    //if win
    if (type == 'W') {
      std::cout << "Congratulations! You have won. Hooray!" << std::endl;
    }
    //if lose
    if (type == 'L') {
      std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
    }
    //if normal page
    if (type == 'N') {
      std::cout << "What would you like to do? " << std::endl;
      std::cout << std::endl;

      for (size_t i = 0; i < c->choices.size(); i++) {
        std::cout << " " << i + 1 << ". " << c->choices[i] << std::endl;
      }
    }
  }
};
