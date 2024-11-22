#include <cstring>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.hpp>
#include <vector>
#include <algorithm>

int utils::ExecuteCommand(std::string command) {
  int argumentCount = std::count(command.begin(), command.end(), ' ') + 1;
  char *cArgs[argumentCount];
  int status;

  char *temp = (char *)command.c_str();
  char *argv[200];
  int argc = 0;
  argv[argc] = std::strtok(temp, " ");
  while (argv[argc]) {
    argc++;
    argv[argc] = std::strtok(0, " ");
  }

  if (fork() == 0) {
    execvp(argv[0], argv);
  } else {
    wait(&status);
  }
  return status;
}

std::vector<std::string>
utils::GetExtensionTypeAndFilename(std::string FullFilePath) {
  std::string intermediate;
  std::vector<std::string> Splits;
  std::stringstream check1(FullFilePath);
  while (getline(check1, intermediate, '.')) {
    Splits.push_back(intermediate);
  }
  return Splits;
}

std::string utils::GetFilenameWithoutExtension(std::string FullFilePath) {
  std::string intermediate;
  std::vector<std::string> Splits;
  std::stringstream check1(FullFilePath);
  while (getline(check1, intermediate, '/')) {
    Splits.push_back(intermediate);
  }
  std::string FilenameWithExtension = (--Splits.end())->data();
  return utils::GetExtensionTypeAndFilename(FilenameWithExtension)[0];
}
