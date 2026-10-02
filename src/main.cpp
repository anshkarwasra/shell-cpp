#include <iostream>
#include <string>
#include <vector>
#include <sstream>



std::vector<std::string> parseCmd(std::string cmd)
{
  std::stringstream cmdStream(cmd);
  std::string token;
  std::vector<std::string> tokens;

  while (cmdStream >> token)
  {
    tokens.push_back(token);
  }
  return tokens;

}


bool evaluateCmd(std::string cmd)
{
  
  if(cmd == "exit")
  {
    return true;
  }
  else if (cmd == "echo")
  {
    return true;
  }
  // would be used to evaluate weather it is valid or not 
  return false;
}


int main() {
  // Flush after every std::cout / std:cerr
  // std::cout << std::unitbuf;
  // std::cerr << std::unitbuf;
  while(true)
  {
    std::cout << "$ ";
    std::cout << std::unitbuf;

    // take the user command as input 
    std::string cmd;
    std::cin >> cmd;

    // process the user's command 
    std::vector<std::string> cmdTokens = parseCmd(cmd);
    if(!evaluateCmd(cmdTokens[0]))
    {
      std::cout<< cmd << ": command not found"<< std::endl;
      std::cout << std::unitbuf;
    }
    if(cmdTokens[0] == "exit")
    {
      break;
    }
    else if(cmdTokens[0] == "echo")
    {
      for(int i =1; i < cmdTokens.size();i++)
      {
        if(i == cmdTokens.size()-1)
        {
          std::cout<<cmdTokens[i];
          
        }
        else 
        {
          std::cout<<cmdTokens[i]<<" ";
        }
      }
    }
  }
  
  return 0;
}
