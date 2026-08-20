#pragma once

#include "core/Resource.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Geometry : public vq::core::Resource {
  public:
    enum class Attribute : uint8_t {
        POSITION = 0,
        NORMAL,
        TEXTURE_COORDINATE,
        ATTRIBUTE_COUNT
    };

    explicit Geometry(const std::string& id,
                      const std::string& source_file_path = "");
    ~Geometry() override = default;

    Geometry(Geometry&& other);
    Geometry& operator=(Geometry&& other);

    static void unbind_all();

    template <Attribute AttributeType>
    inline size_t get_attribute_size() const;
    template <Attribute AttributeType>
    inline void get_attribute(std::vector<float>& data) const;
    template <Attribute AttributeType>
    inline void set_attribute(const std::vector<float>& data);

    inline size_t get_indices_size() const;
    void get_indices(std::vector<unsigned int>& data) const;
    void set_indices(const std::vector<unsigned int>& data);

    void bind() const;
    void unbind() const;

  protected:
    bool do_load() noexcept override;
    bool do_reload() noexcept override;
    void do_unload() noexcept override;

  private:
    void download_attribute_data(std::vector<float>& data,
                                 Attribute attribute) const;
    void upload_attribute_data(const std::vector<float>& data,
                               Attribute attribute);

    std::string m_source_file;
    std::array<size_t, static_cast<uint8_t>(Attribute::ATTRIBUTE_COUNT)>
        m_attribute_sizes;
    size_t m_indices_size;
    std::array<unsigned int, static_cast<uint8_t>(Attribute::ATTRIBUTE_COUNT)>
        m_attributes;
    unsigned int m_vertex_array;
    unsigned int m_index_array;
};

/*----------------------------------------------------------------------------*/

template <Geometry::Attribute AttributeType>
inline size_t Geometry::get_attribute_size() const {
    static_assert(AttributeType != Attribute::ATTRIBUTE_COUNT,
                  "AttributeType has to be a valid Geometry attribute type");
    return m_attribute_sizes[static_cast<uint8_t>(AttributeType)];
}

template <Geometry::Attribute AttributeType>
inline void Geometry::get_attribute(std::vector<float>& data) const {
    static_assert(AttributeType != Attribute::ATTRIBUTE_COUNT,
                  "AttributeType has to be a valid Geometry attribute type");
    this->download_attribute_data(data, AttributeType);
}

template <Geometry::Attribute AttributeType>
inline void Geometry::set_attribute(const std::vector<float>& data) {
    static_assert(AttributeType != Attribute::ATTRIBUTE_COUNT,
                  "AttributeType has to be a valid Geometry attribute type");
    this->upload_attribute_data(data, AttributeType);
    m_attribute_sizes[static_cast<uint8_t>(AttributeType)] = data.size();
}

inline size_t Geometry::get_indices_size() const { return m_indices_size; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
