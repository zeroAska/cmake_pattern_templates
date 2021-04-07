#include "lib2.hpp"

lib2::lib2() {
  std::cout<<"lib2 constructed.\n";
}
lib2::~lib2() {
  std::cout<<"lib2 destructed.\n";
}

void lib2::print_exist_from_lib1() {
  lib1 l1;
  l1.print_exist();
}
