#pragma once

#include <string>
#include <vector>

namespace utils {
int ExecuteCommand(std::string command);
std::vector<std::string> GetExtensionTypeAndFilename(std::string FullFilePath);
std::string GetFilenameWithoutExtension(std::string FullFilePath);
bool FileExists(std::string Filename);
} // namespace utils
