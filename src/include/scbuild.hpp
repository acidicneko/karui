#pragma once

#include <meta.hpp>
#include <string>
#include <vector>

namespace scbuild {
class builder {
public:
  int threads = DEFAULT_THREAD_COUNT;
  std::string compiler;
  std::string target;
  std::string buildFolder;
  std::vector<std::string> compilerOptions;
  void ParserConfig(std::string ConfigFileLocation);
};
} // namespace scbuild
