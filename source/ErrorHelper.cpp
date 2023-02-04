#include <ErrorHelper.hpp>
#include <filesystem>

namespace ErrorHelper {
void SetupDirectories() {
  std::filesystem::create_directories(
      std::filesystem::path("sdmc:/3ds/lpp3dsn/"));
}

void WriteErr(std::string msg) {
  std::ofstream errs("sdmc:/3ds/lpp3dsn/errors.txt", std::ios::app);
  errs << "\nError Happened:\n" << msg << std::endl;
  errs.close();
}
} // namespace ErrorHelper