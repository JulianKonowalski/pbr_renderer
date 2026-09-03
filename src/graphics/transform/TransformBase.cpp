#include "graphics/transform/TransformBase.hpp"

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

TransformBase::TransformBase() : m_transform_matrix(1.0f), m_is_dirty(true) {}

/*----------------------------------------------------------------------------*/

TransformBase::TransformBase(TransformBase&& other)
    : EventEmitter(std::move(other)),
      m_transform_matrix(std::move(other.m_transform_matrix)),
      m_is_dirty(other.m_is_dirty) {}

/*----------------------------------------------------------------------------*/

TransformBase::TransformBase(const TransformBase& other)
    : EventEmitter(other), m_transform_matrix(other.m_transform_matrix),
      m_is_dirty(other.m_is_dirty) {}

/*----------------------------------------------------------------------------*/

TransformBase& TransformBase::operator=(TransformBase&& other) {
    m_transform_matrix = std::move(other.m_transform_matrix);
    m_is_dirty         = other.m_is_dirty;
    EventEmitter::operator=(std::move(other));
    this->emit_event<TransformChangeEvent>(*this);
    return *this;
}

/*----------------------------------------------------------------------------*/

TransformBase& TransformBase::operator=(const TransformBase& other) {
    m_transform_matrix = other.m_transform_matrix;
    m_is_dirty         = other.m_is_dirty;
    EventEmitter::operator=(other);
    this->emit_event<TransformChangeEvent>(*this);
    return *this;
}

/*----------------------------------------------------------------------------*/

void TransformBase::reset_transform() {
    this->on_reset();
    m_transform_matrix = glm::mat4(1.0f);
    this->emit_event<TransformResetEvent>(*this);
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void TransformBase::update_transform_matrix() {
    if (m_is_dirty) {
        this->on_update();
        m_is_dirty = false;
        this->emit_event<TransformUpdateEvent>(*this);
    }
}

/*----------------------------------------------------------------------------*/

const glm::mat4& TransformBase::get_transform_matrix() {
    this->update_transform_matrix();
    return m_transform_matrix;
}

/*----------------------------------------------------------------------------*/

void TransformBase::set_dirty() {
    m_is_dirty = true;
    this->emit_event<TransformChangeEvent>(*this);
}

/*----------------------------------------------------------------------------*/
