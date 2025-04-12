#include "meta.hpp"
#include <build.hpp>
#include <getopt.h>
#include <iostream>

int main(int argc, char **argv) {
  karui::builder *Builder = new class karui::builder();
  Builder->ParserConfig(DEFAULT_CONFIG_FILE);
  option longopts[] = {{"build", no_argument, NULL, 'b'},
                       {"clean", no_argument, NULL, 'c'},
                       {"help", no_argument, NULL, 'h'},
                       {"version", no_argument, NULL, 'v'},
                       {0}};
  while (1) {
    const int opt = getopt_long(argc, argv, "bvc", longopts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
    case 'b':
      build::Build(Builder);
      break;
    case 'c':
      break;
    case 'v':
      std::cout << "Karui v1.0.0; A Simple C Build tool.\n"
                << "Copyright acidicneko 2024 https://github.com/acidicneko"
                << std::endl;
      break;
    default:
      std::cout << "I'm bored." << std::endl;
      break;
    }
  }
  return 0;
}
