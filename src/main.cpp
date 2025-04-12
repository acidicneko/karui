#include "meta.hpp"
#include "utils.hpp"
#include <build.hpp>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

int main(int argc, char **argv) {
  std::string currentConfigFile = DEFAULT_CONFIG_FILE;
  option longopts[] = {{"config", required_argument, NULL, 'c'},
                       {"help", no_argument, NULL, 'h'},
                       {"version", no_argument, NULL, 'v'},
                       {0}};
  while (1) {
    const int opt = getopt_long(argc, argv, "vc:", longopts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
    case 'c':
      currentConfigFile = optarg;
      if(!utils::FileExists(currentConfigFile)){
        std::cout << "No such configuration file exists: " << currentConfigFile << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      std::cout << "Karui " << VERSION << "; A Simple C Build tool.\n"
                << "Copyright acidicneko 2024 https://github.com/acidicneko"
                << std::endl;
      break;
    default:
      break;
    }
  }
  karui::builder *Builder = new class karui::builder();
  Builder->ParserConfig(currentConfigFile);
  build::Build(Builder);
  return 0;
}
