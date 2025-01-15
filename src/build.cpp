#include <build.hpp>
#include <compiler.hpp>
#include <dependency.hpp>
#include <dirent.h>
#include <iostream>
#include <meta.hpp>
#include <string>
#include <utils.hpp>
#include <vector>

std::vector<std::string> build::CollectSourceFiles(std::string ParentDirectory,
                                                   int Level) {
  std::vector<std::string> SourceFiles;
  if (auto dir = opendir(ParentDirectory.c_str())) {
    while (auto f = readdir(dir)) {
      if (f->d_name[0] == '.')
        continue;
      std::string FullFilePath = ParentDirectory + "/" + f->d_name;
      if (f->d_type == DT_DIR) {
        build::CollectSourceFiles(FullFilePath, 1);
      } else {
        if (utils::GetExtensionTypeAndFilename(FullFilePath)[1] == "c" |
            utils::GetExtensionTypeAndFilename(FullFilePath)[1] == "cpp") {
          SourceFiles.push_back(FullFilePath);
        }
      }
    }
    closedir(dir);
  }
  return SourceFiles;
}

int build::Build() {
  compiler::Compiler *Compiler = new class compiler::Compiler();

  Compiler->CompilerName = "g++";
  Compiler->CompilerOptions.push_back("-Isrc/include");
  Compiler->CompilerOptions.push_back("-std=c++20");
  Compiler->BuildFolder = "build";
  Compiler->Target = "zeta";

  std::vector<std::string> SourceFiles = CollectSourceFiles("src", 1);
  std::vector<std::string> ObjectFiles;
  bool CompiledSuccessfully = true;
  for (auto file = SourceFiles.begin(); file != SourceFiles.end(); file++) {
    std::string SrcFileLocation = file->data();

    if (dependency::CheckModified(SrcFileLocation)) {
      if (Compiler->Compile(SrcFileLocation) != 0) {
        CompiledSuccessfully = false;
        std::cout << "\033[1;31mERROR\033[0m: Failed to compile: "
                  << file->data() << std::endl;
        continue;
      }
    }
    std::string ObjFileLocation = SrcFileLocation.replace(0, 3, "build");

    ObjectFiles.push_back(
        SrcFileLocation.replace(ObjFileLocation.find(".", 0, 1) + 1, 3, "o"));
  }
  if (CompiledSuccessfully) {
    Compiler->Link(ObjectFiles);
  } else {
    std::cout
        << "\033[1;31mERROR\033[0m: Failed to link object files due to missing "
           "object files.\n";
  }
  delete Compiler;
  return 0;
}
