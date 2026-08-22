#include "graphics/core/Transform.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace vq::tests::graphics::core::Transform {

/*----------------------------------------------------------------------------*/

static const std::array<float, 4> s_identity_quaternion({1.0f, 0.0f, 0.0f,
                                                         0.0f});
static const std::array<float, 3> s_identity_scale({1.0f, 1.0f, 1.0f});
static const std::array<float, 3> s_identity_translation({0.0f, 0.0f, 0.0f});
static const std::array<float, 16>
    s_identity_transform_matrix({1.0f, 0.0f, 0.0f, 0.0f, //
                                 0.0f, 1.0f, 0.0f, 0.0f, //
                                 0.0f, 0.0f, 1.0f, 0.0f, //
                                 0.0f, 0.0f, 0.0f, 1.0f});

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_is_identity_by_default) {
    vq::graphics::core::Transform transform;

    const auto& transform_rotation_quaternion =
        transform.get_rotation_quaternion();
    const auto& transform_scale       = transform.get_scale();
    const auto& transform_translation = transform.get_translation();
    const auto& transform_matrix      = transform.get_transform_matrix();

    for (int i = 0; i < transform_rotation_quaternion.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_rotation_quaternion[i] -
                              s_identity_quaternion[i]) < 0.000001f);
    }

    for (int i = 0; i < transform_scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_scale[i] - s_identity_scale[i]) <
                    0.000001f);
    }

    for (int i = 0; i < transform_translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_translation[i] -
                              s_identity_translation[i]) < 0.000001f);
    }

    for (int i = 0; i < transform_matrix.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_matrix[i] -
                              s_identity_transform_matrix[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_move_constructor) {
    vq::graphics::core::Transform transform_1;

    const std::array<float, 4> rotation({1.0f, 2.0f, 3.0f, 4.0f});
    const std::array<float, 3> translation({1.0f, -1.0f, 0.0f});
    const std::array<float, 3> scale({0.5f, 0.6f, 0.7f});

    transform_1.set_rotation_quaternion(rotation);
    transform_1.set_translation(translation);
    transform_1.set_scale(scale);

    const auto& transform_1_rotation    = transform_1.get_rotation_quaternion();
    const auto& transform_1_scale       = transform_1.get_scale();
    const auto& transform_1_translation = transform_1.get_translation();

    vq::graphics::core::Transform transform_2(std::move(transform_1));
    const auto& transform_2_rotation    = transform_1.get_rotation_quaternion();
    const auto& transform_2_scale       = transform_1.get_scale();
    const auto& transform_2_translation = transform_1.get_translation();

    for (int i = 0; i < transform_1_rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_1_rotation[i] - rotation[i]) <
                    0.000001f);
        ASSERT_TRUE(std::fabs(transform_2_rotation[i] - rotation[i]) <
                    0.000001f);
    }

    for (int i = 0; i < transform_1_scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_1_scale[i] - scale[i]) < 0.000001f);
        ASSERT_TRUE(std::fabs(transform_2_scale[i] - scale[i]) < 0.000001f);
    }

    for (int i = 0; i < transform_1_translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(transform_1_translation[i] - translation[i]) <
                    0.000001f);
        ASSERT_TRUE(std::fabs(transform_2_translation[i] - translation[i]) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_reset) {
    vq::graphics::core::Transform transform;
    transform.rotate_quaternion({2.0f, 3.0f, 4.0f, 5.0f});
    transform.scale({2.0f, 3.0f, 4.0f});
    transform.translate({1.0f, 2.0f, 3.0f});

    const auto& rotation    = transform.get_rotation_quaternion();
    const auto& scale       = transform.get_scale();
    const auto& translation = transform.get_translation();

    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_quaternion[i]) >
                    0.000001f);
    }

    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) > 0.000001f);
    }

    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) >
                    0.000001f);
    }

    transform.reset();

    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }

    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) < 0.000001f);
    }

    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_set_and_get_rotation_euler_degrees) {
    vq::graphics::core::Transform transform;
    auto rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        // rotation values, especially converted back to euler angles are really
        // imprecise
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    std::array<float, 3> new_rotation({90.0f, 0.0f, 0.0f});
    transform.set_rotation_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 90.0f, 0.0f};
    transform.set_rotation_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.0f, 90.0f};
    transform.set_rotation_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_set_and_get_rotation_euler_radians) {
    vq::graphics::core::Transform transform;
    auto rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.000001f);
    }

    const std::array<float, 3> new_rotation({0.1f, 0.2f, 0.3f});
    transform.set_rotation_euler_radians(new_rotation);
    rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_set_and_get_rotation_quaternion) {
    vq::graphics::core::Transform transform;
    const auto& rotation = transform.get_rotation_quaternion();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }

    const std::array<float, 4> new_rotation({1.0f, 2.0f, 3.0f, 4.0f});
    transform.set_rotation_quaternion(new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_set_and_get_scale) {
    vq::graphics::core::Transform transform;
    const auto& scale = transform.get_scale();
    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) < 0.000001f);
    }

    const std::array<float, 3> new_scale({1.0f, 2.0f, 3.0f});
    transform.set_scale(new_scale);
    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - new_scale[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_set_and_get_translation) {
    vq::graphics::core::Transform transform;
    const auto& translation = transform.get_translation();
    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }

    const std::array<float, 3> new_translation({1.0f, 2.0f, 3.0f});
    transform.set_translation(new_translation);
    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - new_translation[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_get_transform_matrix) {
    vq::graphics::core::Transform transform;

    const std::array<float, 3> new_scale({2.0f, 3.0f, 4.0f});
    transform.reset();
    transform.scale(new_scale);
    const std::array<float, 16> target_scale_matrix({2.0f, 0.0f, 0.0f, 0.0f, //
                                                     0.0f, 3.0f, 0.0f, 0.0f, //
                                                     0.0f, 0.0f, 4.0f, 0.0f, //
                                                     0.0f, 0.0f, 0.0f, 1.0f});
    const auto& scale_transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < scale_transform_matrix.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale_transform_matrix[i] -
                              target_scale_matrix[i]) < 0.000001f);
    }

    const std::array<float, 3> new_translation({1.0f, 2.0f, 3.0f});
    transform.reset();
    transform.translate(new_translation);
    const std::array<float, 16> target_translation_matrix(
        {1.0f, 0.0f, 0.0f, 0.0f,   //
         0.0f, 1.0f, 0.0f, 0.0f,   //
         0.0f, 0.0f, 1.0f, 0.0f,   //
         1.0f, 2.0f, 3.0f, 1.0f}); // row-major form is used
    // matrix is recalculated only when get_transform_matrix is called
    const auto& translation_transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < translation_transform_matrix.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation_transform_matrix[i] -
                              target_translation_matrix[i]) < 0.000001f);
    }

    std::array<float, 3> new_rotation({90.0f, 0.0f, 0.0f});
    transform.reset();
    transform.rotate_euler_degrees(new_rotation);
    std::array<float, 16> target_rotation_matrix({1.0f, 0.0f, 0.0f, 0.0f,  //
                                                  0.0f, 0.0f, 1.0f, 0.0f,  //
                                                  0.0f, -1.0f, 0.0f, 0.0f, //
                                                  0.0f, 0.0f, 0.0f, 1.0f});
    const auto& x_rotation_transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < x_rotation_transform_matrix.size(); ++i) {
        ASSERT_TRUE(std::fabs(x_rotation_transform_matrix[i] -
                              target_rotation_matrix[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 90.0f, 0.0f};
    transform.reset();
    transform.rotate_euler_degrees(new_rotation);
    target_rotation_matrix                  = {0.0f, 0.0f, -1.0f, 0.0f, //
                                               0.0f, 1.0f, 0.0f,  0.0f, //
                                               1.0f, 0.0f, 0.0f,  0.0f, //
                                               0.0f, 0.0f, 0.0f,  1.0f};
    const auto& y_rotation_transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < y_rotation_transform_matrix.size(); ++i) {
        ASSERT_TRUE(std::fabs(y_rotation_transform_matrix[i] -
                              target_rotation_matrix[i]) < 0.000001f);
    }

    new_rotation = {0.0f, 0.0f, 90.0f};
    transform.reset();
    transform.rotate_euler_degrees(new_rotation);
    // z rotations are clockwise, so sin and -sin values are switched
    target_rotation_matrix                  = {0.0f,  1.0f, 0.0f, 0.0f, //
                                               -1.0f, 0.0f, 0.0f, 0.0f, //
                                               0.0f,  0.0f, 1.0f, 0.0f, //
                                               0.0f,  0.0f, 0.0f, 1.0f};
    const auto& z_rotation_transform_matrix = transform.get_transform_matrix();
    for (int i = 0; i < z_rotation_transform_matrix.size(); ++i) {
        std::cout << z_rotation_transform_matrix[i] << " "
                  << target_rotation_matrix[i] << "\n";
        ASSERT_TRUE(std::fabs(z_rotation_transform_matrix[i] -
                              target_rotation_matrix[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_rotate_quaternion) {
    const auto multiply_quaternions =
        [](const std::array<float, 4>& q1,
           const std::array<float, 4>& q2) -> std::array<float, 4> {
        return {
            q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3],
            q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2],
            q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1],
            q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0],
        };
    };

    vq::graphics::core::Transform transform;
    const auto& rotation = transform.get_rotation_quaternion();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }

    std::array<float, 4> new_rotation({1.0f, 0.0f, 0.0f, 0.0f});
    transform.rotate_quaternion(new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - s_identity_quaternion[i]) <
                    0.000001f);
    }

    new_rotation = {1.0f, 1.0f, 0.0f, 0.0f};
    transform.rotate_quaternion(new_rotation);
    auto result = multiply_quaternions(s_identity_quaternion, new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - result[i]) < 0.000001f);
    }

    new_rotation = {1.0f, 0.0f, 1.0f, 0.0f};
    transform.rotate_quaternion(new_rotation);
    result = multiply_quaternions(result, new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - result[i]) < 0.000001f);
    }

    new_rotation = {1.0f, 0.0f, 0.0f, 1.0f};
    transform.rotate_quaternion(new_rotation);
    result = multiply_quaternions(result, new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - result[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_rotate_euler_radians) {
    vq::graphics::core::Transform transform;
    auto rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    transform.rotate_euler_radians({0.0f, 0.0f, 0.0f});
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    std::array<float, 3> new_rotation({0.1f, 0.0f, 0.0f});
    transform.rotate_euler_radians(new_rotation);
    rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {-0.1f, 0.0f, 0.0f};
    transform.rotate_euler_radians(new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.1f, 0.0f};
    transform.rotate_euler_radians(new_rotation);
    rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, -0.1f, 0.0f};
    transform.rotate_euler_radians(new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.0f, 0.1f};
    transform.rotate_euler_radians(new_rotation);
    rotation = transform.get_rotation_euler_radians();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.0f, -0.1f};
    transform.rotate_euler_radians(new_rotation);
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_rotate_euler_degrees) {
    vq::graphics::core::Transform transform;
    auto rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    std::array<float, 3> new_rotation({0.0f, 0.0f, 0.0f});
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    new_rotation = {90.0f, 0.0f, 0.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {-90.0f, 0.0f, 0.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 90.0f, 0.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, -90.0f, 0.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.0f, 90.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i] - new_rotation[i]) < 0.1f);
    }

    new_rotation = {0.0f, 0.0f, -90.0f};
    transform.rotate_euler_degrees(new_rotation);
    rotation = transform.get_rotation_euler_degrees();
    for (int i = 0; i < rotation.size(); ++i) {
        ASSERT_TRUE(std::fabs(rotation[i]) < 0.1f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_scale) {
    vq::graphics::core::Transform transform;
    const auto& scale = transform.get_scale();
    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) < 0.000001f);
    }

    std::array<float, 3> new_scale({0.5f, 0.5f, 0.5f});
    transform.scale(new_scale);
    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - (s_identity_scale[i] * new_scale[i])) <
                    0.000001f);
    }

    new_scale = {2.0f, 2.0f, 2.0f};
    transform.scale(new_scale);
    for (int i = 0; i < scale.size(); ++i) {
        ASSERT_TRUE(std::fabs(scale[i] - s_identity_scale[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_CORE_Transform, transform_translate) {
    vq::graphics::core::Transform transform;
    const auto& translation = transform.get_translation();
    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }

    const std::array<float, 3> new_translation({1.0f, 2.0f, 3.0f});
    transform.translate(new_translation);
    for (int i = 0; i < translation.size(); ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - (s_identity_translation[i] +
                                                new_translation[i])) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::graphics::core::Transform
