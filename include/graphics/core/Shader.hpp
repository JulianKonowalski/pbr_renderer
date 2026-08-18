#pragma once

#include "core/Resource.hpp"

#include <cstdint>
#include <utility>
#include <vector>

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Shader final : public vq::core::Resource {
  public:
    enum class ShaderSourceType : uint8_t { VERTEX, FRAGMENT };

    struct ShaderSourceFile {
        const std::string source_file_path;
        const ShaderSourceType type;
    };

    explicit Shader(const std::string& id,
                    std::vector<ShaderSourceFile>& source_files);
    ~Shader() override;

    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

    static void unbind_all();

    void bind();
    void unbind();

  protected:
    virtual bool do_load() noexcept override;
    virtual bool do_reload() noexcept override;
    virtual void do_unload() noexcept override;

  private:
    static unsigned int s_current_shader;

    std::vector<ShaderSourceFile> m_source_files;
    unsigned int m_shader_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
