#pragma once

#include "core/Component.hpp"
#include "graphics/core/Geometry.hpp"

namespace vq::entity_components {

/*----------------------------------------------------------------------------*/

class MeshComponent final : public vq::core::Component {
  public:
    MeshComponent(vq::core::Entity& parent,
                  vq::graphics::core::Geometry* geometry);
    ~MeshComponent();

    void render() override;
    void update(double delta_time) override {}

  private:
    vq::graphics::core::Geometry*
        m_geometry; // this will be a resource handle someday
};

/*----------------------------------------------------------------------------*/

} // namespace vq::entity_components
