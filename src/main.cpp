#include <build.hpp>
#include <getopt.h>

int main(int argc, char **argv) {
  /*option longopts[] = {{"build", no_argument, NULL, 'b'},
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

      break;
    case 'c':
      break;
    case 'v':
      std::cout << "GitMan v2.0.0; A github based source package manager.\n"
                << "Copyright acidicneko 2024 https://github.com/acidicneko"
                << std::endl;
      break;
    default:
      std::cout << "I'm bored." << std::endl;
      break;
    }
  }*/
  build::Build();
  return 0;
}
