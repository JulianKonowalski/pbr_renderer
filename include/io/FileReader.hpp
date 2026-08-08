#pragma once

#include <string>

namespace vq::io {

/*----------------------------------------------------------------------------*/

template <typename OutputType>
class FileReader {
  public:
    FileReader()  = default;
    ~FileReader() = default;

    virtual void read(OutputType& output,
                      const std::string& file_path) const noexcept = 0;
};

/*----------------------------------------------------------------------------*/

class GLSLReader : public FileReader<std::string> {
  public:
    using FileReader::FileReader;

    void read(std::string& output,
              const std::string& file_path) const noexcept override;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::io
