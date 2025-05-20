#include <Filesystem.hpp>
#include <algorithm>
#include <memory>

namespace D7 {
namespace FS {
void File::ReadDataAt(size_t start, char* buf, size_t size) {
  if (start > this->Size || start + size > this->Size) {
    throw std::runtime_error("File::ReadDataAt: Out of range!");
  }
  std::copy_n(this->Data + start, size, buf);
}

void Write2File(const void* data, const size_t& len, const std::string& path) {
  std::fstream f(path, std::ios::out | std::ios::binary);
  if (!f) {
    throw std::runtime_error("Failed to create file!");
  }
  f.write((const char*)data, len);
  f.close();
}

File ReadFile(const std::string& path) {
  File res;
  std::fstream f(path, std::ios::in | std::ios::binary | std::ios::ate);
  if (!f) {
    throw std::runtime_error("Failed to open file!");
  }
  res.Size = f.tellg();
  f.seekg(0, std::ios::beg);
  res.Data = (char*)new uint8_t[res.Size];
  f.read((char*)res.Data, res.Size);
  f.close();
  return res;
}
}  // namespace FS
}  // namespace D7