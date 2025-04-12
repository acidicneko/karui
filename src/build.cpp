#include <build.hpp>
#include <compiler.hpp>
#include <dependency.hpp>
#include <dirent.h>
#include <iostream>
#include <meta.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
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

int build::Build(karui::builder *Builder) {
  compiler::Compiler *Compiler = new class compiler::Compiler();

  Compiler->CompilerName = Builder->compiler;
  Compiler->CompilerOptions = Builder->compilerOptions;
  Compiler->LinkerOptions = Builder->linkerOptions;
  Compiler->BuildFolder = Builder->buildFolder;
  Compiler->Target = Builder->target;

  std::vector<std::string> SourceFiles = CollectSourceFiles(Builder->srcFolder, 1);
  std::vector<std::string> ObjectFiles;
  bool CompiledSuccessfully = true;

  std::queue<std::string> SourceFilesQueue;
  std::mutex SourceFilesQueueMutex;
  std::mutex ObjectFilesMutex;
  for (const auto &file : SourceFiles) {
    SourceFilesQueue.push(file);
  }

  auto worker = [&]() {
    while (true) {
      std::string file;
      {
        std::lock_guard<std::mutex> lock(SourceFilesQueueMutex);
        if (SourceFilesQueue.empty()) {
          break;
        }
        file = SourceFilesQueue.front();
        SourceFilesQueue.pop();
      }
      
      std::string objFile = file;
      objFile.replace(0, 3, Builder->buildFolder);
      size_t dotPos = objFile.find_last_of(".");
      if (dotPos != std::string::npos) {
        objFile.replace(dotPos + 1, std::string::npos, "o");
      } 
      
      if (dependency::CheckModified(file, objFile)) {
        if (Compiler->Compile(file) != 0) {
          CompiledSuccessfully = false;
          std::cout << "\033[1;31mERROR\033[0m: Failed to compile: " << file
                    << std::endl;
          continue;
        }
      }
      
      {
        std::lock_guard<std::mutex> lock(ObjectFilesMutex); 
        ObjectFiles.push_back(objFile);
      }
    }
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < GetThreads(); i++) {
    threads.emplace_back(std::thread(worker));
  }

  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
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
