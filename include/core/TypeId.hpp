#pragma once

#include <type_traits>

namespace vq::core {

/*----------------------------------------------------------------------------*/

template <typename Base>
class TypeId final {
  public:
    TypeId(void)  = delete;
    ~TypeId(void) = delete;

    template <typename Child>
    static size_t get_type_id(void) {
        static_assert(std::is_base_of_v<Base, Child>,
                      "Child type must derive from Base type");
        static size_t child_type_id = s_type_id_counter++;
        return child_type_id;
    }

  private:
    static size_t s_type_id_counter;
};

/*----------------------------------------------------------------------------*/

template <typename Base>
size_t TypeId<Base>::s_type_id_counter = 0;

/*----------------------------------------------------------------------------*/

} // namespace vq::core
