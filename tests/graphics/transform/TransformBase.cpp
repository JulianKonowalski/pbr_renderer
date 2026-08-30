#include "graphics/transform/TransformBase.hpp"

#include <array>
#include <gtest/gtest.h>

namespace vq::test::graphics::transform::TransformBase {

/*----------------------------------------------------------------------------*/

const std::array<std::array<float, 4>, 4>
    s_identity_matrix({std::array<float, 4>({1.0f, 0.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 1.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 1.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})});

class Transform final : public vq::graphics::transform::TransformBase {
  public:
    Transform()
        : TransformBase(), m_transform_factor(1.0f), m_on_reset_called(false),
          m_on_update_called(false) {}

    Transform(Transform&& other)
        : vq::graphics::transform::TransformBase(std::move(other)) {
        m_transform_factor = other.m_transform_factor;
        m_on_reset_called  = other.m_on_reset_called;
        m_on_update_called = other.m_on_update_called;
    }

    Transform(const Transform& other)
        : vq::graphics::transform::TransformBase(other) {
        m_transform_factor = other.m_transform_factor;
        m_on_reset_called  = other.m_on_reset_called;
        m_on_update_called = other.m_on_update_called;
    }

    Transform& operator=(Transform&& other) {
        vq::graphics::transform::TransformBase::operator=(std::move(other));
        m_transform_factor = other.m_transform_factor;
        m_on_reset_called  = other.m_on_reset_called;
        m_on_update_called = other.m_on_update_called;
        return *this;
    }

    Transform& operator=(const Transform& other) {
        vq::graphics::transform::TransformBase::operator=(other);
        m_transform_factor = other.m_transform_factor;
        m_on_reset_called  = other.m_on_reset_called;
        m_on_update_called = other.m_on_update_called;
        return *this;
    }

    inline bool on_reset_called() const { return m_on_reset_called; }
    inline bool on_update_called() const { return m_on_update_called; }

    float get_transform_factor() const { return m_transform_factor; }
    void set_transform_factor(const float transform_factor) {
        m_transform_factor = transform_factor;
    }

  protected:
    void on_reset() override {
        m_transform_factor = 1.0f;
        m_on_reset_called  = true;
    }

    void on_update() override {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m_transform_matrix[i][j] *= m_transform_factor;
            }
        }
        m_on_update_called = true;
    }

  private:
    float m_transform_factor;
    bool m_on_reset_called;
    bool m_on_update_called;
};

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, is_identity_by_default) {
    Transform transform;
    const auto& transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, move_constructor) {
    Transform transform_1;
    transform_1.reset_transform();
    transform_1.set_transform_factor(2.0f);
    const auto& transform_matrix_1 = transform_1.get_transform_matrix();

    Transform transform_2(std::move(transform_1));
    const auto& transform_matrix_2 = transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    ASSERT_EQ(transform_1.on_reset_called(), transform_2.on_reset_called());
    ASSERT_EQ(transform_1.on_update_called(), transform_2.on_update_called());
    ASSERT_TRUE(std::fabs(transform_1.get_transform_factor() -
                          transform_2.get_transform_factor()) < 0.000001f);
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, copy_constructor) {
    Transform transform_1;
    transform_1.reset_transform();
    transform_1.set_transform_factor(2.0f);
    const auto& transform_matrix_1 = transform_1.get_transform_matrix();

    Transform transform_2(transform_1);
    const auto& transform_matrix_2 = transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    ASSERT_EQ(transform_1.on_reset_called(), transform_2.on_reset_called());
    ASSERT_EQ(transform_1.on_update_called(), transform_2.on_update_called());
    ASSERT_TRUE(std::fabs(transform_1.get_transform_factor() -
                          transform_2.get_transform_factor()) < 0.000001f);
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, move_operator) {
    Transform transform_1;
    transform_1.reset_transform();
    transform_1.set_transform_factor(2.0f);
    const auto& transform_matrix_1 = transform_1.get_transform_matrix();

    Transform transform_2          = std::move(transform_1);
    const auto& transform_matrix_2 = transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    ASSERT_EQ(transform_1.on_reset_called(), transform_2.on_reset_called());
    ASSERT_EQ(transform_1.on_update_called(), transform_2.on_update_called());
    ASSERT_TRUE(std::fabs(transform_1.get_transform_factor() -
                          transform_2.get_transform_factor()) < 0.000001f);
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, copy_operator) {
    Transform transform_1;
    transform_1.reset_transform();
    transform_1.set_transform_factor(2.0f);
    const auto& transform_matrix_1 = transform_1.get_transform_matrix();

    Transform transform_2          = transform_1;
    const auto& transform_matrix_2 = transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    ASSERT_EQ(transform_1.on_reset_called(), transform_2.on_reset_called());
    ASSERT_EQ(transform_1.on_update_called(), transform_2.on_update_called());
    ASSERT_TRUE(std::fabs(transform_1.get_transform_factor() -
                          transform_2.get_transform_factor()) < 0.000001f);
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, dirty_flag_updates) {
    {
        Transform transform;
        ASSERT_TRUE(transform.is_dirty());
        transform.get_transform_matrix();
        ASSERT_FALSE(transform.is_dirty());
    }
    {
        Transform transform;
        ASSERT_TRUE(transform.is_dirty());
        transform.update_transform_matrix();
        ASSERT_FALSE(transform.is_dirty());
        transform.reset_transform();
        ASSERT_TRUE(transform.is_dirty());
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, resets_to_identity_transform) {
    Transform transform;
    transform.set_transform_factor(2.0f);
    const auto& transform_matrix = transform.get_transform_matrix();

    bool equal = true;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::fabs(transform_matrix[i][j] - s_identity_matrix[i][j]) >
                0.000001f) {
                equal = false;
                break;
            }
        }
    }

    ASSERT_FALSE(equal);

    transform.reset_transform();
    transform.update_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, calls_on_reset) {
    Transform transform;
    ASSERT_FALSE(transform.on_reset_called());
    transform.reset_transform();
    ASSERT_TRUE(transform.on_reset_called());
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TransformBase, calls_on_update) {
    Transform transform;
    ASSERT_FALSE(transform.on_update_called());
    transform.update_transform_matrix();
    ASSERT_TRUE(transform.on_update_called());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::test::graphics::transform::TransformBase
