#include <build.hpp>
#include <compiler.hpp>
#include <iostream>
#include <karui.hpp>
#include <string>
#include <utils.hpp>
#include <yaml.h>

karui::hook *GetHook(karui::builder &Builder, std::string HookName) {
  for (auto &hook : Builder.hooks) {
    if (hook.name == HookName) {
      return &hook;
    }
  }
  return nullptr;
}

void karui::RunHook(karui::builder &Builder, const std::string &HookName) {
  karui::hook *hook = GetHook(Builder, HookName);
  if (!hook) {
    std::cout << "\033[1;31mERROR\033[0m: Hook '" << HookName
              << "' not found in configuration." << std::endl;
    return;
  }
  for (const auto &command : hook->commands) {
    if (Builder.verbose) {
      std::cout << "\033[1;33mExecuting command: " << command << "\033[0m"
                << std::endl;
    }
    int status = utils::ExecuteCommand(command);
    if (status != 0) {
      std::cout << "Command failed with status: " << status << std::endl;
    }
  }
}

void LinkHook(karui::builder &Builder, std::vector<std::string> &ObjectFiles) {
  if (ObjectFiles.empty()) {
    std::cout << "\033[1;31mERROR\033[0m: No object files to link."
              << "Was link hook called before build hook?" << std::endl;
    return;
  }
  if (!Builder.extraObjs.empty()) {
    for (const auto &extraObj : Builder.extraObjs) {
      ObjectFiles.push_back(extraObj);
    }
  }
  compiler::Compiler *Compiler = new class compiler::Compiler();
  Compiler->CompilerName = Builder.compiler;
  Compiler->CompilerOptions = Builder.compilerOptions;
  Compiler->LinkerOptions = Builder.linkerOptions;
  Compiler->BuildFolder = Builder.buildFolder;
  Compiler->Target = Builder.target;
  Compiler->Verbose = Builder.verbose;
  Compiler->Link(ObjectFiles);

  delete Compiler;
}

void karui::RunWorkflow(karui::builder &Builder, const std::string &HookName) {
  std::vector<std::string> ObjectFiles;
  if (Builder.workflow.empty()) {
    std::cout << "\033[1;31mERROR\033[0m: No workflow defined in configuration."
              << std::endl;
    return;
  }
  if (HookName != "build" && HookName != "link" && HookName != "none") {
    karui::RunHook(Builder, HookName);
    return;
  }
  for (const auto &hookName : Builder.workflow) {
    std::cout << "\033[1;34mRunning hook: " << hookName << "\033[0m"
              << std::endl;
    if (hookName == "build") {
      build::Build(Builder, ObjectFiles);
    } else if (hookName == "link") {
      LinkHook(Builder, ObjectFiles);
    } else {
      karui::RunHook(Builder, hookName);
    }
  }
}
