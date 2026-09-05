#pragma once

#include "graphics/core/Texture.hpp"

namespace vq::graphics::texture {

/*----------------------------------------------------------------------------*/

class Texture2D final : public vq::graphics::core::Texture {
  public:
    Texture2D(const std::string& id, const std::string& source_file);
    ~Texture2D() = default;

    Texture2D(Texture2D&& other);
    Texture2D& operator=(Texture2D&& other);

    static void unbind_all();
    inline static Texture2D* const get_current();

    void set_wrap_x(const WrapType wrap_type) noexcept override;
    void set_wrap_y(const WrapType wrap_type) noexcept override;
    void set_wrap_z(const WrapType wrap_type) noexcept override;

    void set_filter_min(const FilterType filter_type) noexcept override;
    void set_filter_mag(const FilterType filter_type) noexcept override;

    void bind() noexcept override;
    void unbind() noexcept override;

  protected:
    bool do_load() noexcept override;
    bool do_reload() noexcept override;
    void do_unload() noexcept override;

  private:
    static Texture2D* s_current_texture;
};

/*----------------------------------------------------------------------------*/

inline Texture2D* const Texture2D::get_current() { return s_current_texture; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::texture
