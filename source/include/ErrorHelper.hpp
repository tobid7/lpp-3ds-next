#pragma once

#include <string>
#include <fstream>

namespace ErrorHelper
{
    void SetupDirectories();
    void WriteErr(std::string msg);
}