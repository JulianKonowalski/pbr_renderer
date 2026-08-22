#pragma once

#include "core/Resource.hpp"

#include <array>
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
    ~Shader() override = default;

    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

    static void unbind_all();

    void bind();
    void unbind();

    unsigned int get_uniform_location(const std::string& uniform_id);
    void set_uniform_mat4(const unsigned int uniform_location,
                          const std::array<float, 16>& data);

  protected:
    virtual bool do_load() noexcept override;
    virtual bool do_reload() noexcept override;
    virtual void do_unload() noexcept override;

  private:
    std::vector<ShaderSourceFile> m_source_files;
    unsigned int m_shader_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
