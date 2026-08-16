#pragma once

#include "core/Resource.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace vq::entity_components {
class MeshComponent;
}

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Geometry : vq::core::Resource {

    friend class vq::entity_components::MeshComponent; /** TODO: implement an
                                                         actual renderer */

  public:
    enum AttributeType : uint8_t {
        POSITION = 0,
        NORMAL,
        TEXTURE_COORDINATES,
        INDEX,
        COUNT
    };

    static Geometry box_geometry(const std::array<float, 3>& min_vertex,
                                 const std::array<float, 3>& max_vertex,
                                 const std::string& id);

    static Geometry plane_geometry(const std::array<float, 2>& min_vertex,
                                   const std::array<float, 2>& max_vertex,
                                   const std::string& id);

    static Geometry sphere_geometry(const float radius,
                                    const unsigned int num_stacks,
                                    const unsigned int num_sectors,
                                    const std::string& id);

    explicit Geometry(const std::string& id,
                      const std::string& asset_path = "");
    ~Geometry() override;

    Geometry(Geometry&& other);
    Geometry& operator=(Geometry&& other);

  protected:
    virtual bool do_load() noexcept override;
    virtual bool do_reload() noexcept override;
    virtual void do_unload() noexcept override;

    std::array<unsigned int, AttributeType::COUNT> m_buffers;
    std::array<size_t, AttributeType::COUNT> m_buffer_sizes;
    unsigned int m_vertex_array;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
