#include "lib1.hpp"


lib1::lib1() {
  std::cout<<"lib1 constructed\n";
}

lib1::~lib1() {
  std::cout<<"lib1 destructed\n";
}

void lib1::print_exist() {
  if ( !boost::filesystem::exists( "CMakeLists.txt" ) )
    std::cout<<"lib1: CMakeLists.txt doesn't exist in the curr folder\n";
  else
    std::cout<<"lib1: CMakeLists.txt exist in the curr folder\n";
}
