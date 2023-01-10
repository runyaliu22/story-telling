#include <fstream>   //file
#include <iostream>  //std
#include <vector>

#include "story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Invalid number of input argument!" << std::endl;
    return EXIT_FAILURE;
  }

  //read the story(page creation)
  std::vector<page *> pages = read_the_story(argv[1]); 

  //print pages
  print_pages(pages);

  //delete pages
  delete_pages(pages);

  return EXIT_SUCCESS;
}
