#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.hpp>
#include <vector>

int utils::ExecuteCommand(std::string command) {
  int status;

  // Make a mutable copy for strtok_r
  std::vector<char> cmdBuffer(command.begin(), command.end());
  cmdBuffer.push_back('\0');

  char *argv[200];
  int argc = 0;
  char *saveptr;
  argv[argc] = strtok_r(cmdBuffer.data(), " ", &saveptr);
  while (argv[argc]) {
    argc++;
    argv[argc] = strtok_r(nullptr, " ", &saveptr);
  }

  if (fork() == 0) {
    execvp(argv[0], argv);
    _exit(127);
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

bool utils::FileExists(std::string Filename) {
  std::ifstream File(Filename);
  if (!File) {
    return false;
  }
  return true;
}
