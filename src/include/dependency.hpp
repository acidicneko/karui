#pragma once

#include <string>

namespace dependency {
bool CheckModified(std::string File);
void UpdateMeta(std::string File);
} // namespace dependency
