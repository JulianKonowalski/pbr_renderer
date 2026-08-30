#include "graphics/transform/RotateTransform.hpp"

#include <array>
#include <gtest/gtest.h>

namespace vq::tests::graphics::transform::RotateTransform {

/*----------------------------------------------------------------------------*/

const std::array<std::array<float, 4>, 4>
    s_identity_matrix({std::array<float, 4>({1.0f, 0.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 1.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 1.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})});

const std::array<float, 4> s_identity_quaternion({0.0f, 0.0f, 0.0f, 1.0f});

const std::array<float, 3> s_identity_euler_angles({0.0f, 0.0f, 0.0f});

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, is_identity_by_default) {
    vq::graphics::transform::RotateTransform rotate_transform;
    const auto& transform_matrix = rotate_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, move_constructor) {
    vq::graphics::transform::RotateTransform rotate_transform_1;
    rotate_transform_1.rotate_quaternion({1.0f, 2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = rotate_transform_1.get_transform_matrix();

    vq::graphics::transform::RotateTransform rotate_transform_2(
        std::move(rotate_transform_1));
    const auto& transform_matrix_2 = rotate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& quaternion_1 = rotate_transform_1.get_rotation_quaternion();
    const auto& quaternion_2 = rotate_transform_2.get_rotation_quaternion();
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion_1[i] - quaternion_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, copy_constructor) {
    vq::graphics::transform::RotateTransform rotate_transform_1;
    rotate_transform_1.rotate_quaternion({1.0f, 2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = rotate_transform_1.get_transform_matrix();

    vq::graphics::transform::RotateTransform rotate_transform_2(
        rotate_transform_1);
    const auto& transform_matrix_2 = rotate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& quaternion_1 = rotate_transform_1.get_rotation_quaternion();
    const auto& quaternion_2 = rotate_transform_2.get_rotation_quaternion();
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion_1[i] - quaternion_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, move_operator) {
    vq::graphics::transform::RotateTransform rotate_transform_1;
    rotate_transform_1.rotate_quaternion({1.0f, 2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = rotate_transform_1.get_transform_matrix();

    vq::graphics::transform::RotateTransform rotate_transform_2 =
        std::move(rotate_transform_1);
    const auto& transform_matrix_2 = rotate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& quaternion_1 = rotate_transform_1.get_rotation_quaternion();
    const auto& quaternion_2 = rotate_transform_2.get_rotation_quaternion();
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion_1[i] - quaternion_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, copy_operator) {
    vq::graphics::transform::RotateTransform rotate_transform_1;
    rotate_transform_1.rotate_quaternion({1.0f, 2.0f, 3.0f, 4.0f});
    const auto& transform_matrix_1 = rotate_transform_1.get_transform_matrix();

    vq::graphics::transform::RotateTransform rotate_transform_2 =
        rotate_transform_1;
    const auto& transform_matrix_2 = rotate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& quaternion_1 = rotate_transform_1.get_rotation_quaternion();
    const auto& quaternion_2 = rotate_transform_2.get_rotation_quaternion();
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion_1[i] - quaternion_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, sets_and_gets_rotation_quaternion) {
    // vq::graphics::transform::RotateTransform rotate_transform;
    // const auto& quaternion = rotate_transform.get_rotation_quaternion();
    // for (int i = 0; i < 4; ++i) {
    //     ASSERT_TRUE(std::fabs(quaternion[i] - s_identity_quaternion[i]) <
    //                 0.000001f);
    // }

    // const std::array<float, 4> new_rotation({1.0f, 2.0f, 3.0f, 4.0f});
    /**
     * GLM quaternions are broken - they're stored and retrieved in xyzw order,
     * but constructed in wxyz order, so if the quaternion argument is
     * constructed in place, the ordering has to be changed manually.
     * */
    // rotate_transform.set_rotation_quaternion(
    //     {new_rotation[3], new_rotation[0], new_rotation[1],
    //     new_rotation[2]});
    // for (int i = 0; i < 4; ++i) {
    //     ASSERT_TRUE(std::fabs(quaternion[i] - new_rotation[i]) < 0.000001f);
    // }

    // rotate_transform.set_rotation_quaternion({1.0f, 0.0f, 0.0f, 0.0f});
    // rotate_transform.rotate_quaternion({1.0f, 1.0f, 0.0f, 0.0f});
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, sets_and_gets_rotation_euler_radians) {
    vq::graphics::transform::RotateTransform rotate_transform;
    auto rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_euler_angles[i]) <
                    0.000001f);
    }

    std::array<float, 3> new_rotation({1.0f, 0.0f, 0.0f});
    rotate_transform.set_rotation_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 1.0f, 0.0f};
    rotate_transform.set_rotation_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 0.0f, 1.0f};
    rotate_transform.set_rotation_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_radians({0.0f, 0.0f, 0.0f});
    new_rotation = {1.0f, 0.0f, 0.0f};
    rotate_transform.rotate_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_radians({0.0f, 0.0f, 0.0f});
    new_rotation = {0.0f, 1.0f, 0.0f};
    rotate_transform.rotate_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_radians({0.0f, 0.0f, 0.0f});
    new_rotation = {0.0f, 0.0f, 1.0f};
    rotate_transform.rotate_euler_radians(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_radians();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, sets_and_gets_rotation_euler_degrees) {
    vq::graphics::transform::RotateTransform rotate_transform;
    auto rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_euler_angles[i]) <
                    0.000001f);
    }

    std::array<float, 3> new_rotation({1.0f, 0.0f, 0.0f});
    rotate_transform.set_rotation_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 1.0f, 0.0f};
    rotate_transform.set_rotation_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 0.0f, 1.0f};
    rotate_transform.set_rotation_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_degrees({0.0f, 0.0f, 0.0f});
    new_rotation = {1.0f, 0.0f, 0.0f};
    rotate_transform.rotate_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_degrees({0.0f, 0.0f, 0.0f});
    new_rotation = {0.0f, 1.0f, 0.0f};
    rotate_transform.rotate_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }

    rotate_transform.set_rotation_euler_degrees({0.0f, 0.0f, 0.0f});
    new_rotation = {0.0f, 0.0f, 1.0f};
    rotate_transform.rotate_euler_degrees(
        {new_rotation[0], new_rotation[1], new_rotation[2]});
    rotation = rotate_transform.get_rotation_euler_degrees();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, rotates_on_axis_radians) {}
TEST(GRAPHICS_TRANSFORM_RotateTransform, rotates_on_axis_degrees) {}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, limits_rotation_to_2_pi_range) {}
TEST(GRAPHICS_TRANSFORM_RotateTransform, limits_rotation_to_360_degre_range) {}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, gets_transform_matrix) {}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_RotateTransform, resets_to_identity_transform) {
    vq::graphics::transform::RotateTransform rotate_transform;
    const auto& transform_matrix = rotate_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    const auto& quaternion = rotate_transform.get_rotation_quaternion();
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }

    rotate_transform.rotate_euler_degrees({1.0f, 2.0f, 3.0f});
    rotate_transform.update_transform_matrix();

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
    for (int i = 0; i < 4; ++i) {
        if (std::fabs(quaternion[i] - s_identity_quaternion[i]) > 0.000001f) {
            equals = false;
            break;
        }
    }

    ASSERT_FALSE(equals);

    rotate_transform.reset_transform();
    rotate_transform.update_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(std::fabs(quaternion[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::graphics::transform::RotateTransform
