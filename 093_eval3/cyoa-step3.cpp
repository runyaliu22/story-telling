#include <algorithm>
#include <fstream>   //file
#include <iostream>  //std
#include <stack>
#include <vector>

#include "story.h"

int main(int argc, char ** argv) {
  //read the story(page creation)
  std::vector<page *> pages = read_the_story(argv[1]);

  //check that every page that is referenced by a choice is valid
  check_page_reference(pages);

  //check that there's at least on win/lose page
  check_W_L(pages);

  //check that every page is referenced
  check_every_page_referenced(pages);

  //find the winning paths
  dfs(pages);

  //delete pages
  delete_pages(pages);

  return EXIT_SUCCESS;
}
