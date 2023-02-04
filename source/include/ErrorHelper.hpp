#pragma once

#include <fstream>
#include <string>

namespace ErrorHelper {
void SetupDirectories();
void WriteErr(std::string msg);
} // namespace ErrorHelper