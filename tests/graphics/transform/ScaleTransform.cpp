#include "graphics/transform/ScaleTransform.hpp"

#include <array>
#include <gtest/gtest.h>

namespace vq::test::graphics::transform::ScaleTransform {

/*----------------------------------------------------------------------------*/

const std::array<std::array<float, 4>, 4>
    s_identity_matrix({std::array<float, 4>({1.0f, 0.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 1.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 1.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})});

const std::array<float, 3> s_identity_scale({1.0f, 1.0f, 1.0f});

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, is_identity_by_default) {
    vq::graphics::transform::ScaleTransform scale_transform;
    const auto& transform_matrix = scale_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, move_constructor) {
    vq::graphics::transform::ScaleTransform scale_transform_1;
    scale_transform_1.scale({2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = scale_transform_1.get_transform_matrix();

    vq::graphics::transform::ScaleTransform scale_transform_2(
        std::move(scale_transform_1));
    const auto& transform_matrix_2 = scale_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& scale_1 = scale_transform_1.get_scale();
    const auto& scale_2 = scale_transform_2.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale_1[i] - scale_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, copy_constructor) {
    vq::graphics::transform::ScaleTransform scale_transform_1;
    scale_transform_1.scale({2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = scale_transform_1.get_transform_matrix();

    vq::graphics::transform::ScaleTransform scale_transform_2(
        scale_transform_1);
    const auto& transform_matrix_2 = scale_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& scale_1 = scale_transform_1.get_scale();
    const auto& scale_2 = scale_transform_2.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale_1[i] - scale_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, move_operator) {
    vq::graphics::transform::ScaleTransform scale_transform_1;
    scale_transform_1.scale({2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = scale_transform_1.get_transform_matrix();

    vq::graphics::transform::ScaleTransform scale_transform_2 =
        std::move(scale_transform_1);
    const auto& transform_matrix_2 = scale_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& scale_1 = scale_transform_1.get_scale();
    const auto& scale_2 = scale_transform_2.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale_1[i] - scale_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, copy_operator) {
    vq::graphics::transform::ScaleTransform scale_transform_1;
    scale_transform_1.scale({2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = scale_transform_1.get_transform_matrix();

    vq::graphics::transform::ScaleTransform scale_transform_2 =
        scale_transform_1;
    const auto& transform_matrix_2 = scale_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& scale_1 = scale_transform_1.get_scale();
    const auto& scale_2 = scale_transform_2.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale_1[i] - scale_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, sets_and_gets_scale) {
    vq::graphics::transform::ScaleTransform scale_transform;
    const auto& scale = scale_transform.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) < 0.000001f);
    }

    const std::array<float, 3> new_scale({2.0f, 3.0f, 4.0f});
    scale_transform.set_scale({new_scale[0], new_scale[1], new_scale[2]});
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - new_scale[i]) < 0.000001f);
    }

    scale_transform.scale({new_scale[0], new_scale[1], new_scale[2]});
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - (new_scale[i] * new_scale[i])) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, gets_transform_matrix) {
    vq::graphics::transform::ScaleTransform scale_transform;
    const auto& transform_matrix = scale_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    scale_transform.scale({2.0f, 3.0f, 4.0f});
    scale_transform.update_transform_matrix();
    std::array<std::array<float, 4>, 4> target_transform_matrix(
        {std::array<float, 4>({2.0f, 0.0f, 0.0f, 0.0f}), //
         std::array<float, 4>({0.0f, 3.0f, 0.0f, 0.0f}), //
         std::array<float, 4>({0.0f, 0.0f, 4.0f, 0.0f}), //
         std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})});

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  target_transform_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_ScaleTransform, resets_to_identity_transform) {
    vq::graphics::transform::ScaleTransform scale_transform;
    const auto& transform_matrix = scale_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    const auto& scale = scale_transform.get_scale();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale[i] < s_identity_scale[i]) < 0.000001f);
    }

    scale_transform.scale({2.0f, 3.0f, 4.0f});
    scale_transform.update_transform_matrix();

    bool equals = true;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::fabs(transform_matrix[i][j] - s_identity_matrix[i][j]) >
                0.000001f) {
                equals = false;
                break;
            }
        }
    }
    ASSERT_FALSE(equals);

    equals = true;
    for (int i = 0; i < 3; ++i) {
        if (std::fabs(scale[i] - s_identity_scale[i]) > 0.000001f) {
            equals = false;
            break;
        }
    }
    ASSERT_FALSE(equals);

    scale_transform.reset_transform();
    scale_transform.update_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(scale[i] < s_identity_scale[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::test::graphics::transform::ScaleTransform
