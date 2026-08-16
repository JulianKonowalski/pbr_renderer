#pragma once

#include "core/Resource.hpp"
#include "graphics/core/Shader.hpp"

#include <vector>

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

/**
 * This will be taking in a vector of strings with shader ids that is should use
 * - gotta wait for the resource manager tho
 */
class Program : public vq::core::Resource {
  public:
    explicit Program(const std::string& id,
                     const std::vector<Shader*>& shaders);
    ~Program() override;

    Program(Program&& other);
    Program& operator=(Program&& other);

    static void unbind_all();

    void bind();
    void unbind();

  protected:
    virtual bool do_load() noexcept override;
    virtual bool do_reload() noexcept override;
    virtual void do_unload() noexcept override;

  private:
    static unsigned int s_current_program;

    std::vector<Shader*> m_shaders;
    unsigned int m_program_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
