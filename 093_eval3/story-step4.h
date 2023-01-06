#include <limits.h>
#include <stdlib.h>

#include <algorithm>
#include <fstream>   //file
#include <iostream>  //std
#include <stack>
#include <vector>

#include "page-step4.h"

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

//read the story into the page class
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

      size_t found = s.find("@");

      //get the page number

      std::string c = s.substr(0, found);

      const char * b = c.c_str();

      size_t num = strtoul(b, &end, 10);

      //check if the page number is valid
      check_page_number(b, errno, end);

      //get the page name

      std::string p = "/" + s.substr(found + 3);

      //get the type of the page

      char t = s[found + 1];

      //check if page declarations are in order

      if (num != pages.size()) {
        std::cerr << "Page declarations must appear in order." << std::endl;
        exit(EXIT_FAILURE);
      }

      //add new page

      pages.push_back(new page(directory, p, t));
    }

    if (s.find("$") != std::string::npos) {
      size_t found0 = s.find("$");

      std::string c = s.substr(0, found0);

      //page number

      char * end = NULL;

      size_t num = strtoul(c.c_str(), &end, 10);

      //check if the page number is valid
      check_page_number(c.c_str(), errno, end);

      //check variable declared after page
      if (num + 1 > pages.size()) {
        std::cerr << "Variables must be declared after page declaration!" << std::endl;
        exit(EXIT_FAILURE);
      }

      //variable name

      size_t found1 = s.find("=");

      std::string d = s.substr(found0 + 1, found1 - found0 - 1);

      std::string e = s.substr(found1 + 1);

      //variable value

      end = NULL;

      long int value = strtol(e.c_str(), &end, 10);

      check_page_number(e.c_str(), errno, end);

      //add pair to mymap(vector of pair used to store variables of each page)

      pages[num]->addpair(d, value);
    }

    //choices creation

    if (s.find(":") != s.find_last_of(":")) {
      if (s.find("[") != std::string::npos) {
        size_t found0 = s.find("[");
        size_t found1 = s.find("]");
        size_t found2 = s.find("=");

        std::string a = s.substr(0, found0);

        //page number

        char * end = NULL;

        size_t num = strtoul(a.c_str(), &end, 10);

        check_page_number(a.c_str(), errno, end);

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

        //mystring

        std::string mystring = s.substr(found0 + 1, found2 - found0 - 1);

        std::string b = s.substr(found2 + 1, found1 - found2 - 1);

        //myvalue

        end = NULL;

        long int myvalue = strtol(b.c_str(), &end, 10);

        check_page_number(b.c_str(), errno, end);

        //show that the page is the condition-checking page

        pages[num]->set_flag();

        //add the conditions to the condition map

        pages[num]->addconditions(mystring, myvalue);

        size_t first = s.find(":");

        size_t last = s.find_last_of(":");

        //add choice text to this page

        pages[num]->addchoices(s.substr(last + 1));

        //add choice destination page to this page

        end = NULL;

        pages[num]->adddestinations(
            strtoul(s.substr(first + 1, last - first - 1).c_str(), &end, 10));
        check_page_number(s.substr(first + 1, last - first - 1).c_str(), errno, end);
      }

      else {
        char * end = NULL;

        size_t first = s.find(":");

        size_t last = s.find_last_of(":");

        std::string a = (s.substr(0, first));

        const char * b = a.c_str();

        //the page number that has choices

        size_t num = strtoul(b, &end, 10);

        //check if the page number is valid
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

        //add choice destination page to this page

        end = NULL;

        pages[num]->adddestinations(
            strtoul(s.substr(first + 1, last - first - 1).c_str(), &end, 10));

        //check if the page number is valid
        check_page_number(s.substr(first + 1, last - first - 1).c_str(), errno, end);

        //happens when the line if a normal choice(either in a non-condition-checking page or in a condition-checking page but with a normal condition)
        pages[num]->addconditions("", LONG_MAX);
      }
    }
  }

  return pages;
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

  std::map<std::string, long int> mymap;
  std::map<std::string, long int>::iterator it;

  while (true) {
    //get the variables of the current page
    std::vector<std::pair<std::string, long int> > temp0 = pages[j]->getmymap();

    std::vector<int> unavailable_index;
    std::vector<int>::iterator it_int;
    //add the varaibles that has been seen so far to mymap, which is a bag to collect these variables
    //could have error, need to update
    if (temp0.size() != 0) {
      for (size_t i = 0; i < temp0.size(); i++) {
        it = mymap.find(temp0[i].first);
        if (it != mymap.end()) {
          it->second = temp0[i].second;
        }
        else {
          mymap.insert(temp0[i]);
        }
      }
    }

    //reaching the page to check the condition

    if (pages[j]->get_flag() == 1) {
      std::vector<std::pair<std::string, long int> > temp1 =
          pages[j]->getchoice()->conditionmap;

      for (size_t i = 0; i < temp1.size(); i++) {
        if (temp1[i].second == LONG_MAX) {
          continue;
        }
        it = mymap.find(temp1[i].first);
        if (it != mymap.end()) {
          if (it->second != temp1[i].second) {
            unavailable_index.push_back(i);
          }
        }
        else {
          if (temp1[i].second != 0) {
            unavailable_index.push_back(i);
          }
        }
      }

      pages[j]->printpage_unavailable(unavailable_index);
    }
    else {
      //print current page
      pages[j]->printpage();
    }

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

    it_int = find(unavailable_index.begin(), unavailable_index.end(), a - 1);

    while (it_int != unavailable_index.end()) {
      std::cout << "That choice is not available at this time, please try again"
                << std::endl;
      std::string s;
      std::getline(std::cin, s);

      a = strtoul(s.c_str(), NULL, 10);
      it_int = find(unavailable_index.begin(), unavailable_index.end(), a - 1);
    }

    //the next page number given the input choice
    j = (pages[j]->getchoice())->destinations[a - 1];  //copy???
  }
}
