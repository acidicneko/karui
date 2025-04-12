#pragma once

#include <string>

namespace dependency {
bool CheckModified(std::string srcFile, std::string objFile);
void UpdateMeta(std::string File);
} // namespace dependency
