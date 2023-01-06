#include <fstream>   //file
#include <iostream>  //std
#include <vector>
class page {
 private:
  //store relative information about choices of each page
  class choice {
   public:
    std::vector<std::string> choices;  //choices of each page
    std::vector<size_t> destinations;  //destinations of each page
  };

  choice * c;

  std::string dn;  //directory name
  std::string pn;  //page name
  char type;

 public:
  page(std::string _dn, std::string _pn, char _type) :
      c(new choice()), dn(_dn), pn(_pn), type(_type) {}

  //deep copy constructor
  page(const page & rhs) :
      c(new choice(*(rhs.c))), dn(rhs.dn), pn(rhs.pn), type(rhs.type) {}

  //assignment operator
  page & operator=(const page & rhs) {
    if (this != &rhs) {
      dn = rhs.dn;
      pn = rhs.pn;
      type = rhs.type;
      delete c;
      c = new choice(*(rhs.c));
    }
    return *this;
  }

  //destructor
  ~page() { delete c; }

  char gettype() { return type; }

  choice * getchoice() { return c; }

  void addchoices(std::string addstring) { c->choices.push_back(addstring); }

  void adddestinations(size_t adddestination) {
    c->destinations.push_back(adddestination);
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
