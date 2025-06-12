#pragma once

#include <meta.hpp>
#include <string>
#include <vector>

namespace karui {

class hook {
public:
  std::string name;
  std::vector<std::string> commands;
};

class builder {
public:
  int threads = DEFAULT_THREAD_COUNT;
  std::string compiler;
  std::string target;
  std::string buildFolder;
  std::string srcFolder;
  std::vector<std::string> compilerOptions;
  std::vector<std::string> linkerOptions;
  std::vector<hook> hooks;
  std::vector<std::string> workflow;
  std::vector<std::string> extraObjs;
  bool verbose = false;
  void ParserConfig(std::string ConfigFileLocation);
};

void RunHook(karui::builder &Builder, const std::string &HookName);
void RunWorkflow(karui::builder &Builder, const std::string &HookName);

} // namespace karui
