#pragma once

#include "core/Resource.hpp"

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Texture : public vq::core::Resource {
  public:
    enum class WrapType {
        REPEAT,
        MIRROR_REPEAT,
        CLAMP_TO_EDGE,
        MIRROR_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class FilterType { NEAREST, LINEAR };

    explicit Texture(const std::string& id, const std::string& source_file);
    ~Texture() override = default;

    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    virtual void set_wrap_x(const WrapType wrap_type) noexcept = 0;
    virtual void set_wrap_y(const WrapType wrap_type) noexcept = 0;
    virtual void set_wrap_z(const WrapType wrap_type) noexcept = 0;

    virtual void set_filter_min(const FilterType filter_type) noexcept = 0;
    virtual void set_filter_mag(const FilterType filter_type) noexcept = 0;

    virtual void bind() noexcept   = 0;
    virtual void unbind() noexcept = 0;

  protected:
    std::string m_source_file;
    unsigned int m_texture_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
