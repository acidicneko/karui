#include "karui.hpp"
#include "meta.hpp"
#include "utils.hpp"
#include <build.hpp>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

int main(int argc, char **argv) {
  std::string currentConfigFile = DEFAULT_CONFIG_FILE;

  std::string manualHookName = "none";
  option longopts[] = {{"config", required_argument, NULL, 'c'},
                       {"help", no_argument, NULL, 'h'},
                       {"version", no_argument, NULL, 'v'},
                       {"run", required_argument, NULL, 'r'},
                       {0}};
  while (1) {
    const int opt = getopt_long(argc, argv, "vc:hr:", longopts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
    case 'c':
      currentConfigFile = optarg;
      if (!utils::FileExists(currentConfigFile)) {
        std::cout << "No such configuration file exists: " << currentConfigFile
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      std::cout << "Karui " << VERSION << "; A Simple C Build tool.\n"
                << "Copyright acidicneko 2024 https://github.com/acidicneko"
                << std::endl;
      exit(EXIT_SUCCESS);
      break;
    case 'h':
      std::cout
          << "Usage: karui [options]\n"
          << "Options:\n"
          << "  -c, --config <file>    Specify the configuration file to use.\n"
          << "  -v, --version          Show version information.\n"
          << "  -r, --run <hook>       Run a specific hook by name.\n"
          << "  -h, --help             Show this help message." << std::endl;
      exit(EXIT_SUCCESS);
      break;
    case 'r':
      manualHookName = optarg;
      break;
    default:
      exit(EXIT_FAILURE);
      break;
    }
  }
  karui::builder *Builder = new class karui::builder();
  Builder->ParserConfig(currentConfigFile);
  karui::RunWorkflow(*Builder, manualHookName);
  delete Builder;
  return 0;
}
