#pragma once

#include <karui.hpp>
#include <string>
#include <vector>

namespace build {

std::vector<std::string> CollectSourceFiles(std::string ParentDirectory,
                                            int Level);
int Build(karui::builder *Builder);
} // namespace build
