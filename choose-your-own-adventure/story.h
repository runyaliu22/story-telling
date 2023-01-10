#include <stdlib.h>

#include <algorithm>
#include <cerrno>
#include <fstream>   //file
#include <iostream>  //std
#include <map>
#include <stack>
#include <vector>

#include "page.h"

//check that every page that is referenced by a choice is valid
void check_page_reference(std::vector<page *> pages) {
  for (size_t i = 0; i < pages.size(); i++) {
    std::vector<size_t> destinations = pages[i]->getchoice()->destinations;
    for (size_t j = 0; j < destinations.size(); j++) {
      if ((destinations[j] < 0) || (destinations[j] >= pages.size())) {
        std::cerr << "Invalid page referenced by a choice!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

//check that there's at least one win/lose page
void check_W_L(std::vector<page *> pages) {
  int flag_W = 0;
  int flag_L = 0;
  for (size_t i = 0; i < pages.size(); i++) {
    if (pages[i]->gettype() == 'W') {
      flag_W = 1;
    }
    if (pages[i]->gettype() == 'L') {
      flag_L = 1;
    }
  }

  if ((flag_W == 0) || (flag_L == 0)) {
    std::cerr << "No win/lose page!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

//check that every page is referenced
void check_every_page_referenced(std::vector<page *> pages) {
  std::map<size_t, int> mymap;
  for (size_t i = 0; i < pages.size(); i++) {
    std::vector<size_t> destinations = pages[i]->getchoice()->destinations;
    for (size_t j = 0; j < destinations.size(); j++) {
      if (destinations[j] != i) {
        mymap.insert(std::pair<size_t, int>(destinations[j], 0));
      }
    }
  }

  if (mymap.size() + 1 != pages.size()) {
    std::cerr << "No every page is referenced!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

//check if the page number is valid
void check_page_number(const char * b, int errnoo, char * end) {
  if (errnoo == ERANGE) {
    std::cout << "Ouf of Range!" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (*b == '\0' || *end != '\0') {
    std::cout << "Failed to convert the entire string!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

//read the story
std::vector<page *> read_the_story(std::string directory) {
  //store pointers to the page objects in a vector
  std::vector<page *> pages;

  //directory of story.txt file
  std::string story_directory = directory + "/story.txt";

  std::ifstream infile;

  infile.open(story_directory.c_str());

  std::string s;

  while (std::getline(infile, s)) {
    //page creations
    if (s.find("@") != std::string::npos) {
      char * end = NULL;

      //find the index of @
      std::size_t found = s.find("@");

      //get the page number
      std::string c = s.substr(0, found);

      const char * b = c.c_str();

      size_t num = strtoul(b, &end, 10);

      //check if the page number is valid
      check_page_number(b, errno, end);

      //get the page name
      std::string d = "/" + s.substr(found + 3);

      //get the type of the page
      char t = s[found + 1];

      //check if page declarations are in order

      if (num != pages.size()) {
        std::cerr << "Page declarations must appear in order." << std::endl;
        exit(EXIT_FAILURE);
      }

      //add new page
      pages.push_back(new page(directory, d, t));
    }

    //choices creation
    if (s.find(":") != s.find_last_of(":")) {
      char * end = NULL;

      size_t first = s.find(":");

      size_t last = s.find_last_of(":");

      std::string a = s.substr(0, first);

      const char * b = a.c_str();

      //the page number that has choices
      size_t num = strtoul(b, &end, 10);

      check_page_number(b, errno, end);

      //check if win/lose page has any choices
      if (pages[num]->gettype() == 'W' || pages[num]->gettype() == 'L') {
        std::cerr << "Win and Lose pages MUST NOT have any choices." << std::endl;
        exit(EXIT_FAILURE);
      }

      //check choices declared after page
      if (num + 1 > pages.size()) {
        std::cerr << "Choices must be declared after page declaration!" << std::endl;
        exit(EXIT_FAILURE);
      }

      //add choice text to this page
      pages[num]->addchoices(s.substr(last + 1));

      end = NULL;
      std::string c = s.substr(first + 1, last - first - 1);
      const char * d = c.c_str();
      size_t des = strtoul(d, &end, 10);

      check_page_number(d, errno, end);

      //add choice destination page to this page
      pages[num]->adddestinations(des);
    }
  }

  return pages;
}

//print the pages
void print_pages(std::vector<page *> pages) {
  std::cout<<"print!!!!"<<std::endl;
  for (size_t i = 0; i < pages.size(); i++) {
    std::cout << "Page " << i << std::endl;
    std::cout << "==========" << std::endl;
    pages[i]->printpage();
  }
}

//delete the pages
void delete_pages(std::vector<page *> pages) {
  for (size_t i = 0; i < pages.size(); i++) {
    delete pages[i];
  }
}

//print the story driven by user input choices
void print_story(std::vector<page *> pages) {
  //store page number
  int j = 0;

  while (true) {
    //print current page
    pages[j]->printpage();

    //break if current page is W or L
    if ((pages[j]->gettype() == 'W') || (pages[j]->gettype() == 'L')) {
      break;
    }

    //store input choice
    std::string s;

    std::getline(std::cin, s);

    //input choice number
    size_t a = strtoul(s.c_str(), NULL, 10);

    //b needs to be be less than or equal to the size of destination and greater than 0
    size_t b = (pages[j]->getchoice())->destinations.size();

    //loop until the input choice is a valid number
    while ((a < 1) || (a > b)) {
      std::cout << "That is not a valid choice, please try again" << std::endl;

      std::string s;

      std::getline(std::cin, s);

      a = strtoul(s.c_str(), NULL, 10);
    }

    //the next page number given the input choice
    j = (pages[j]->getchoice())->destinations[a - 1];  //copy???
  }
}

void dfs(std::vector<page *> pages) {
  //dfs

  //a stack of vectors of sequence of destination pages
  std::stack<std::vector<size_t> > todo;

  //store the visited pages of each route
  std::vector<size_t> visited;
  std::vector<size_t> start;
  //store the current route
  std::vector<size_t> current;
  //store the winning routes
  std::vector<std::vector<size_t> > found;

  //start state on todo is 0, since the beginning of the story is page0
  start.push_back(0);
  todo.push(start);

  //a stack of vectors of sequence of the index+1(1,2,3,...) of destination pages
  std::vector<std::vector<size_t> > found_index;
  std::stack<std::vector<size_t> > todo_index;

  std::vector<size_t> current_index;

  size_t previous_size = 0;

  while (todo.size() != 0) {
    current = todo.top();

    size_t current_size = current.size();

    //current_size < previous_size means that we're currently on another branch instead of going deeper
    if (current_size < previous_size) {
      size_t a = previous_size - current_size + 1;

      while (a > 0) {
        visited.pop_back();
        a--;
      }
    }

    previous_size = current_size;

    todo.pop();

    if (todo_index.size() > 0) {
      current_index = todo_index.top();
      todo_index.pop();
    }

    size_t last = current[current.size() - 1];

    //check if last page is the winning page
    if (pages[last]->gettype() == 'W') {
      found.push_back(current);
      found_index.push_back(current_index);
    }

    else {
      //if not winning page, check if it's visited, if not, add the neighbor pages to todo vector
      if (std::find(visited.begin(), visited.end(), last) == visited.end()) {
        visited.push_back(last);

        std::vector<size_t> dest = (pages[last]->getchoice())->destinations;

        for (size_t i = 0; i < dest.size(); i++) {
          current_index.push_back(i + 1);
          todo_index.push(current_index);
          current_index.pop_back();

          //push for todo
          current.push_back(dest[i]);
          todo.push(current);
          current.pop_back();
        }
      }
    }
  }

  //print in the required format
  for (size_t i = 0; i < found.size(); i++) {
    for (size_t j = 0; j < found[i].size() - 1; j++) {
      std::cout << found[i][j] << "(" << found_index[i][j] << "),";
    }
    std::cout << found[i][found[i].size() - 1] << "(win)" << std::endl;
  }

  if (found.size() == 0) {
    std::cout << "This story is unwinnable!" << std::endl;
  }
}
