#include "io.hpp"
#include <fstream>
#include <stdexcept>

namespace artemis::utils {
std::vector<char> IO::read_file(const std::string& file) {
    std::ifstream f(file, std::ios::ate | std::ios::binary);

    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::vector<char> buffer(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

    f.close();
    return buffer;
}
} // namespace artemis::utils
