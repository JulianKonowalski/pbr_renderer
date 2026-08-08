#include "io/FileReader.hpp"

#include <fstream>

using namespace vq::io;

/*----------------------------------------------------------------------------*/

void GLSLReader::read(std::string& output,
                      const std::string& file_path) const noexcept {
    output.clear();

    std::ifstream file(file_path);
    if (!file.is_open()) {
        return;
    }

    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    output.resize(file_size, ' ');
    file.seekg(0);

    file.read(&output[0], file_size);
}

/*----------------------------------------------------------------------------*/
