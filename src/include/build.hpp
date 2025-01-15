#pragma once

#include <scbuild.hpp>
#include <string>
#include <vector>

namespace build {

std::vector<std::string> CollectSourceFiles(std::string ParentDirectory,
                                            int Level);
int Build(scbuild::builder *Builder);
} // namespace build
