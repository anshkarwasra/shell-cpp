#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>




struct Command {
  std::string cmd;
  std::string options;
  std::vector<std::string> arguments;
  std::vector<std::string> tokens;
};




static bool isExecutable(const std::string &filepath) {
  struct stat st;
  return stat(filepath.c_str(), &st) == 0 && S_ISREG(st.st_mode) &&
         access(filepath.c_str(), X_OK) == 0;
}


#include <sys/wait.h>
#include <unistd.h>

int runProgram(const std::string &fullPath, const std::vector<std::string> &args) {
  std::vector<char *> argv;
  argv.push_back(const_cast<char *>(args[0].c_str()));
  for (size_t i = 1; i < args.size(); i++)
    argv.push_back(const_cast<char *>(args[i].c_str()));
  argv.push_back(nullptr);

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }
  if (pid == 0) {
    // child: replace this process with the program
    execv(fullPath.c_str(), argv.data());
    perror("execv"); // only reached if execv failed
    _exit(127);
  }
  // parent: wait for the child to finish
  int status = 0;
  waitpid(pid, &status, 0);
  return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

static std::vector<std::string> parsePaths(const std::string &pathStr) {
  std::vector<std::string> dirs;
  std::stringstream ss(pathStr);
  std::string dir;
  while (std::getline(ss, dir, ':')) {
    if (!dir.empty())
      dirs.push_back(dir);
  }
  return dirs;
}


static std::string findInPath(const std::string &name,
                              const std::vector<std::string> &dirs) {
  for (const auto &dir : dirs) {
    std::string full = dir + "/" + name;
    if (isExecutable(full))
      return full;
  }
  return "";
}

static bool isBuiltin(const std::string &cmd) {
  static const std::unordered_set<std::string> builtins = {"exit", "echo",
                                                           "type"};
  return builtins.count(cmd) > 0;
}

static Command parseCmd(const std::string &line) {
  Command c;
  std::istringstream in(line);
  std::string token;
  if (in >> c.cmd) {
    c.tokens.push_back(c.cmd);
    while (in >> token) {
      c.tokens.push_back(token);
      if (token[0] == '-')
        c.options = token;
      else
        c.arguments.push_back(token);
    }
  }
  return c;
}

int main() {
  const char *pathEnv = std::getenv("PATH");
  std::vector<std::string> paths = parsePaths(pathEnv ? pathEnv : "");

  std::string line;
  while (true) {
    std::cout << "$ " << std::flush;

    // EOF (Ctrl+D / end of piped input) ends the shell.
    if (!std::getline(std::cin, line))
      break;

    Command c = parseCmd(line);
    if (c.cmd.empty())
      continue;

    if (c.cmd == "exit") {
      break;
    } else if (c.cmd == "echo") {
      for (size_t i = 0; i < c.arguments.size(); i++) {
        if (i > 0)
          std::cout << " ";
        std::cout << c.arguments[i];
      }
      std::cout << std::endl;
    } else if (c.cmd == "type") {
      for (const auto &arg : c.arguments) {
        if (isBuiltin(arg)) {
          std::cout << arg << " is a shell builtin" << std::endl;
          continue;
        }
        std::string full = findInPath(arg, paths);
        if (!full.empty())
          std::cout << arg << " is " << full << std::endl;
        else
          std::cout << arg << ": not found" << std::endl;
      }
    } else {
      std::string full = findInPath(c.cmd, paths);
      if(!full.empty())
      {
        // the command is a executable execute it with its args 
        runProgram(full,c.tokens);

      }
      else 
      {
        std::cout << c.cmd << ": command not found" << std::endl;
      }
    }
  }
  return 0;
}