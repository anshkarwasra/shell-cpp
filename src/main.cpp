#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::string> splitStr(std::string &str, std::string delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0;
  size_t end = str.find(delimiter);

  while (end != std::string::npos) {
    // Add the found token (skipping empty ones if you have double spaces)
    if (end != start) {
      tokens.push_back(str.substr(start, end - start));
    }
    start = end + delimiter.length();
    end = str.find(delimiter, start);
  }
  // Add the last token
  if (start < str.length()) {
    tokens.push_back(str.substr(start));
  }
  return tokens;
}

struct cmdStructure {
  std::string cmd;
  std::string options;
  std::vector<std::string> arguments;

  cmdStructure(std::string c, std::string o, std::vector<std::string> a)
      : cmd(c), options(o), arguments(a) {}
};

struct pathStruct {
  std::string origPath;
  std::string progName;
  pathStruct(std::string inpPath) {
    // Use double quotes for strings
    std::vector<std::string> pathTokens = splitStr(inpPath, "/");
    this->progName = pathTokens[pathTokens.size() - 1];
    this->origPath = inpPath;
  }
};
bool isValidCmd(std::string cmd) {
  std::unordered_set<std::string> validCmds = {"exit", "echo", "type"};
  if (validCmds.find(cmd) != validCmds.end()) {

    return true;
  }

  return false;
}

cmdStructure *parseCmd(std::string cmd) {
  std::stringstream cmdStream(cmd);
  std::string token;
  std::string command;
  std::string options;
  std::vector<std::string> arguments;

  if (std::getline(cmdStream, command, ' ')) {

    while (std::getline(cmdStream, token, ' ')) {
      if (token.empty())
        continue;

      if (token[0] == '-') {
        options = token;
      } else {
        arguments.push_back(token);
      }
    }
  }

  return new cmdStructure(command, options, arguments);
}

std::vector<pathStruct> parsePaths(std::string pathStr) {
  std::stringstream pathStream(pathStr);
  std::string token;
  std::vector<pathStruct> paths;

  while (std::getline(pathStream, token, ':')) {
    paths.push_back(pathStruct(token));
  }
  return paths;
}

void initCmd() { std::cout << "$ " << std::unitbuf; }

int main(int argc, char *argv[]) {

  std::string path_env = std::getenv("PATH");
  std::vector<pathStruct> paths = parsePaths(path_env);
  while (true) {
    initCmd();

    // take the user command as input
    std::string cmd;
    std::getline(std::cin, cmd);

    // process the user's command
    cmdStructure *inpStruct = parseCmd(cmd);
    bool isCmd = isValidCmd(inpStruct->cmd);
    if (!isCmd) {
      std::cout << inpStruct->cmd << ": command not found" << std::endl;
      std::cout << std::unitbuf;
    }
    if (inpStruct->cmd == "exit") {
      break;
    } else if (inpStruct->cmd == "echo") {
      for (int i = 0; i < inpStruct->arguments.size(); i++) {

        std::cout << inpStruct->arguments[i] << " " << std::unitbuf;
      }
      std::cout << std::endl << std::unitbuf;
    } else if (inpStruct->cmd == "type") {
      for (int i = 0; i < inpStruct->arguments.size(); i++) {
        if (!isValidCmd(inpStruct->arguments[i])) {
          if (paths.size() != 0) {
            for (auto path : paths) {
              if (path.progName == inpStruct->arguments[i]) {
                std::cout << inpStruct->arguments[i] << " is " << path.origPath
                          << std::endl
                          << std::unitbuf;
              }
            }
          } else {
            std::cout << inpStruct->arguments[i] << ": not found" << std::endl
                      << std::unitbuf;
          }
        } else {
          std::cout << inpStruct->arguments[i] << " is a shell builtin"
                    << std::endl
                    << std::unitbuf;
        }
      }
    }
    delete inpStruct;
  }

  return 0;
}
