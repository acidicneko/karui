# Karui: A Simple C Build Tool
### Formerly known as scbuild

There are tools like CMake, Ninja, make etc. but they are not as simple, but complex in nature for a relatively simpler project.

karui aims to be easy to use and fast. Note that it is not a replacement for CMake or Ninja, but just a simple build tool for small projects.
It supports both C and C++.


> [!CAUTION]
> This tool is still in development and is not meant to be used in production.

## Features
- Simple to use
- fast
- Recompile only on changes

## Dependencies
- libyaml
Install it using your package manager, for example on Ubuntu/Debian:
```
sudo apt install libyaml-dev
```
On Arch Linux:
```
sudo pacman -S libyaml
```
On Fedora:
```
sudo dnf install libyaml-devel
```

## Install instructions
Clone the repository

```
git clone https://github.com/acidicneko/karui.git
```

Run the provided `configure.sh` with root priveleges, to compile karui. The resultant binary is placed in `build/` folder, which can
be used to compile karui itself with given `karui.yaml` file in project root with the following command
```
karui
```

## Usage
To compile a project, create a `karui.yaml` file in the root of your project. Here is an example:

```yaml
cc: gcc
flags: 
    - -Wall
    - -Wextra
    - -Werror
    - -O2
threads: 4      # Number of threads to use for parallel compilation

ldflags:
    - -lm       # Link with the math library

verbose: true   # Set to true to enable verbose output

buildDir: build  # Directory where the final binary will be placed
srcDir: src      # Directory where the source files are located

extraObjs:      # Extra object files to link with which are not compiled by karui
    - src/main.o

hooks:
    - name: pull
      command: git pull

workflow:
    - pull
    - build     # hook provided by karui (compiles the source files)
    - link      # hook provided by karui (links the final binary)

```

## Future plans
- File exclusions
