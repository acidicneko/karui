#pragma once

#include <meta.hpp>
#include <string>
#include <vector>

namespace karui {
class builder {
public:
  int threads = DEFAULT_THREAD_COUNT;
  std::string compiler;
  std::string target;
  std::string buildFolder;
  std::string srcFolder;
  std::vector<std::string> compilerOptions;
  std::vector<std::string> linkerOptions;
  void ParserConfig(std::string ConfigFileLocation);
};
} // namespace karui
