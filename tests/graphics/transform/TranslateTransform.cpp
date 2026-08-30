#include "graphics/transform/TranslateTransform.hpp"

#include <array>
#include <gtest/gtest.h>

namespace vq::tests::graphics::transform::TranslateTransform {

/*----------------------------------------------------------------------------*/

const std::array<std::array<float, 4>, 4>
    s_identity_matrix({std::array<float, 4>({1.0f, 0.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 1.0f, 0.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 1.0f, 0.0f}), //
                       std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})});

const std::array<float, 3> s_identity_translation({0.0f, 0.0f, 0.0f});

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, is_identity_by_default) {
    vq::graphics::transform::TranslateTransform translate_transform;
    const auto& transform_matrix = translate_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, move_constructor) {
    vq::graphics::transform::TranslateTransform translate_transform_1;
    translate_transform_1.translate({1.0f, 2.0f, 3.0f});
    const auto& transform_matrix_1 =
        translate_transform_1.get_transform_matrix();

    vq::graphics::transform::TranslateTransform translate_transform_2(
        std::move(translate_transform_1));
    const auto& transform_matrix_2 =
        translate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& translation_1 = translate_transform_1.get_translation();
    const auto& translation_2 = translate_transform_2.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation_1[i] - translation_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, copy_constructor) {
    vq::graphics::transform::TranslateTransform translate_transform_1;
    translate_transform_1.translate({1.0f, 2.0f, 3.0f});
    const auto& transform_matrix_1 =
        translate_transform_1.get_transform_matrix();

    vq::graphics::transform::TranslateTransform translate_transform_2(
        translate_transform_1);
    const auto& transform_matrix_2 =
        translate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& translation_1 = translate_transform_1.get_translation();
    const auto& translation_2 = translate_transform_2.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation_1[i] - translation_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, move_operator) {
    vq::graphics::transform::TranslateTransform translate_transform_1;
    translate_transform_1.translate({1.0f, 2.0f, 3.0f});
    const auto& transform_matrix_1 =
        translate_transform_1.get_transform_matrix();

    vq::graphics::transform::TranslateTransform translate_transform_2 =
        std::move(translate_transform_1);
    const auto& transform_matrix_2 =
        translate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& translation_1 = translate_transform_1.get_translation();
    const auto& translation_2 = translate_transform_2.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation_1[i] - translation_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, copy_operator) {
    vq::graphics::transform::TranslateTransform translate_transform_1;
    translate_transform_1.translate({1.0f, 2.0f, 3.0f});
    const auto& transform_matrix_1 =
        translate_transform_1.get_transform_matrix();

    vq::graphics::transform::TranslateTransform translate_transform_2 =
        translate_transform_1;
    const auto& transform_matrix_2 =
        translate_transform_2.get_transform_matrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix_1[i][j] -
                                  transform_matrix_2[i][j]) < 0.000001f);
        }
    }

    const auto& translation_1 = translate_transform_1.get_translation();
    const auto& translation_2 = translate_transform_2.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation_1[i] - translation_2[i]) < 0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, sets_and_gets_translation) {
    vq::graphics::transform::TranslateTransform translate_transform;
    const auto& translation = translate_transform.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }

    const std::array<float, 3> new_translation({1.0f, 2.0f, 3.0f});
    translate_transform.set_translation(
        {new_translation[0], new_translation[1], new_translation[2]});
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - new_translation[i]) < 0.000001f);
    }

    translate_transform.translate(
        {new_translation[0], new_translation[1], new_translation[2]});
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - (2.0f * new_translation[i])) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, gets_transform_matrix) {
    vq::graphics::transform::TranslateTransform translate_transform;
    const auto& transform_matrix = translate_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    translate_transform.translate({1.0f, 2.0f, 3.0f});
    translate_transform.update_transform_matrix();
    std::array<std::array<float, 4>, 4> target_transform_matrix(
        {std::array<float, 4>({1.0f, 0.0f, 0.0f, 0.0f}), //
         std::array<float, 4>({0.0f, 1.0f, 0.0f, 0.0f}), //
         std::array<float, 4>({0.0f, 0.0f, 1.0f, 0.0f}), //
         std::array<float, 4>({1.0f, 2.0f, 3.0f, 1.0f})});

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  target_transform_matrix[i][j]) < 0.000001f);
        }
    }
}

/*----------------------------------------------------------------------------*/

TEST(GRAPHICS_TRANSFORM_TranslateTransform, resets_to_identity_transform) {
    vq::graphics::transform::TranslateTransform translate_transform;
    const auto& transform_matrix = translate_transform.get_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    const auto& translation = translate_transform.get_translation();
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }

    translate_transform.translate({1.0f, 2.0f, 3.0f});
    translate_transform.update_transform_matrix();

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
        if (std::fabs(translation[i] - s_identity_translation[i]) > 0.000001f) {
            equals = false;
            break;
        }
    }

    ASSERT_FALSE(equals);

    translate_transform.reset_transform();
    translate_transform.update_transform_matrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_TRUE(std::fabs(transform_matrix[i][j] -
                                  s_identity_matrix[i][j]) < 0.000001f);
        }
    }

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(std::fabs(translation[i] - s_identity_translation[i]) <
                    0.000001f);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::graphics::transform::TranslateTransform
