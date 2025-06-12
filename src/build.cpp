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
        auto SubFiles = build::CollectSourceFiles(FullFilePath, 1);
        SourceFiles.insert(SourceFiles.end(), SubFiles.begin(), SubFiles.end());
      } else {
        if (utils::GetExtensionTypeAndFilename(FullFilePath)[1] == "c" ||
            utils::GetExtensionTypeAndFilename(FullFilePath)[1] == "cpp") {
          SourceFiles.push_back(FullFilePath);
        }
      }
    }
    closedir(dir);
  }
  return SourceFiles;
}

int build::Build(karui::builder &Builder,
                 std::vector<std::string> &ObjectFiles) {

  std::vector<std::string> SourceFiles =
      CollectSourceFiles(Builder.srcFolder, 1);
  bool CompiledSuccessfully = true;

  std::queue<std::string> SourceFilesQueue;
  std::mutex SourceFilesQueueMutex;
  std::mutex ObjectFilesMutex;
  for (const auto &file : SourceFiles) {
    SourceFilesQueue.push(file);
  }

  auto worker = [&]() {
    compiler::Compiler *ThreadCompiler = new class compiler::Compiler();
    ThreadCompiler->CompilerName = Builder.compiler;
    ThreadCompiler->CompilerOptions = Builder.compilerOptions;
    ThreadCompiler->BuildFolder = Builder.buildFolder;
    ThreadCompiler->Target = Builder.target;
    ThreadCompiler->Verbose = Builder.verbose;

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
      objFile = ThreadCompiler->BuildFolder + "/" +
                utils::GetFilenameWithoutExtension(file) + ".o";

      if (dependency::CheckModified(file, objFile)) {
        if (ThreadCompiler->Compile(file) != 0) {
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
    delete ThreadCompiler;
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < Builder.threads; i++) {
    threads.emplace_back(std::thread(worker));
  }

  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  return CompiledSuccessfully ? 0 : 1;
}
