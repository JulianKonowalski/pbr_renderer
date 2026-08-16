#pragma once

#include "core/Resource.hpp"

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Shader final : public vq::core::Resource {

    friend class Program;

  public:
    enum class ShaderType { VERTEX, FRAGMENT };

    explicit Shader(const std::string& id, const std::string& asset_path,
                    const ShaderType shader_type);
    ~Shader() override;

    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

  protected:
    bool do_load() noexcept override;
    bool do_reload() noexcept override;
    void do_unload() noexcept override;

  private:
    unsigned int m_shader_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
