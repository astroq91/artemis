#pragma once

#include <string>
#include <vector>
namespace artemis::utils {
class IO {
  public:
    static std::vector<char> read_file(const std::string& file);
};
} // namespace artemis::utils
