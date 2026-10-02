#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  std::string cmd;
  std::cin >> cmd;
  std::cout<< cmd << ": command not found";
  
  std::cout << "$ ";
  return 0;
}
