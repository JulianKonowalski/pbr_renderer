#pragma once

#include <type_traits>

namespace vq::core {

/*----------------------------------------------------------------------------*/

template <typename BaseType>
class TypeId final {
  public:
    TypeId()  = delete;
    ~TypeId() = delete;

    template <typename ChildType>
    static size_t get_type_id() {
        static_assert(std::is_base_of_v<BaseType, ChildType>,
                      "Child type must derive from Base type");
        static size_t child_type_id = s_type_id_counter++;
        return child_type_id;
    }

  private:
    static size_t s_type_id_counter;
};

/*----------------------------------------------------------------------------*/

template <typename BaseType>
size_t TypeId<BaseType>::s_type_id_counter = 0;

/*----------------------------------------------------------------------------*/

} // namespace vq::core
