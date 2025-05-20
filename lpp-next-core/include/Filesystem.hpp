#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace D7 {
namespace FS {
class File {
 public:
  File() = default;

  ~File() { Clean(); }

  void ReadDataAt(size_t start, char* buf, size_t size);
  void Clean() {
    if (Data) {
      delete[] Data;
    }
    Size = 0;
    Data = 0;
  }
  size_t Size = 0;
  char* Data = 0;
};
void Write2File(const void* data, const size_t& len, const std::string& path);
File ReadFile(const std::string& path);
}  // namespace FS
}  // namespace D7