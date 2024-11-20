#pragma once

#include <string>
#include <vector>

namespace build {

std::vector<std::string> CollectSourceFiles(std::string ParentDirectory,
                                            int Level);
int Build();
} // namespace build
