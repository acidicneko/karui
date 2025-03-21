#pragma once

#include <string>
#include <vector>

namespace compiler {

class Compiler {
public:
  std::string CompilerName = "none";
  std::vector<std::string> CompilerOptions;
  std::vector<std::string> LinkerOptions;
  std::string Target;
  std::string BuildFolder;

  int Compile(std::string SourceFile);
  int Link(std::vector<std::string> ObjectFiles);
};
} // namespace compiler
