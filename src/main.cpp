#include <iostream>
#include <string>

bool evaluateCmd(std::string cmd)
{
  // would be used to evaluate weather it is valid or not 
  return false;
}


int main() {
  // Flush after every std::cout / std:cerr
  while(true)
  {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::cout << "$ ";

    // take the user command as input 
    std::string cmd;
    std::cin >> cmd;

    // process the user's command 
    if(!evaluateCmd(cmd))
    {
      std::cout<< cmd << ": command not found";
    }
  }
  
  return 0;
}
