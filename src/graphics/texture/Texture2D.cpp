#include "graphics/texture/Texture2D.hpp"

#include <glad/glad.h>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

using namespace vq::graphics::texture;

/*----------------------------------------------------------------------------*/

Texture2D* Texture2D::s_current_texture = nullptr;

/*----------------------------------------------------------------------------*/

Texture2D::Texture2D(const std::string& id, const std::string& source_file)
    : vq::graphics::core::Texture(id, source_file) {}

/*----------------------------------------------------------------------------*/

Texture2D::Texture2D(Texture2D&& other)
    : vq::graphics::core::Texture(std::move(other)) {
    if (&other == s_current_texture) {
        s_current_texture = this;
    }
}

/*----------------------------------------------------------------------------*/

Texture2D& Texture2D::operator=(Texture2D&& other) {
    vq::graphics::core::Texture::operator=(std::move(other));
    if (&other == s_current_texture) {
        s_current_texture = this;
    }
    return *this;
}

/*----------------------------------------------------------------------------*/

void Texture2D::unbind_all() {
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    s_current_texture = nullptr;
}

/*----------------------------------------------------------------------------*/

void Texture2D::set_wrap_x(const WrapType wrap_type) noexcept {
    auto* const current = s_current_texture;
    if (current != this) {
        this->bind();
    }

    switch (wrap_type) {
    case WrapType::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        break;
    case WrapType::MIRROR_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        break;
    case WrapType::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case WrapType::MIRROR_CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_MIRROR_CLAMP_TO_EDGE);
        break;
    case WrapType::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        break;
    }

    if (current != this) {
        current->bind();
    }
}

/*----------------------------------------------------------------------------*/

void Texture2D::set_wrap_y(const WrapType wrap_type) noexcept {
    auto* const current = s_current_texture;
    if (current != this) {
        this->bind();
    }

    switch (wrap_type) {
    case WrapType::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case WrapType::MIRROR_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    case WrapType::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case WrapType::MIRROR_CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_MIRROR_CLAMP_TO_EDGE);
        break;
    case WrapType::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        break;
    }

    if (current != this) {
        current->bind();
    }
}

/*----------------------------------------------------------------------------*/

void Texture2D::set_wrap_z(const WrapType wrap_type) noexcept {
    auto* const current = s_current_texture;
    if (current != this) {
        this->bind();
    }

    switch (wrap_type) {
    case WrapType::REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        break;
    case WrapType::MIRROR_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
        break;
    case WrapType::CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;
    case WrapType::MIRROR_CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R,
                        GL_MIRROR_CLAMP_TO_EDGE);
        break;
    case WrapType::CLAMP_TO_BORDER:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        break;
    }

    if (current != this) {
        current->bind();
    }
}

/*----------------------------------------------------------------------------*/

void Texture2D::set_filter_min(const FilterType filter_type) noexcept {
    auto* const current = s_current_texture;
    if (current != this) {
        this->bind();
    }

    switch (filter_type) {
    case FilterType::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case FilterType::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    }

    if (current != this) {
        current->bind();
    }
}

/*----------------------------------------------------------------------------*/

void Texture2D::set_filter_mag(const FilterType filter_type) noexcept {
    auto* const current = s_current_texture;
    if (current != this) {
        this->bind();
    }

    switch (filter_type) {
    case FilterType::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case FilterType::NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    }

    if (current != this) {
        current->bind();
    }
}

/*----------------------------------------------------------------------------*/

void Texture2D::bind() noexcept {
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    s_current_texture = this;
}

/*----------------------------------------------------------------------------*/

void Texture2D::unbind() noexcept {
    if (s_current_texture == this) {
        Texture2D::unbind_all();
    }
}

/*----------------------------------------------------------------------------*/

bool Texture2D::do_load() noexcept {
    int width = 0, height = 0, components = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data =
        stbi_load(m_source_file.c_str(), &width, &height, &components, 4);
    if (!data) {
        std::cout << "Failed to read image file: " << m_source_file << "\n";
        return false;
    }

    glGenTextures(1, &m_texture_id);
    if (!m_texture_id) {
        std::cout << "Failed to create a GL Texture\n";
        stbi_image_free(data);
        return false;
    }

    this->bind();
    this->set_wrap_x(WrapType::REPEAT);
    this->set_wrap_y(WrapType::REPEAT);
    this->set_wrap_z(WrapType::REPEAT);
    this->set_filter_min(FilterType::LINEAR);
    this->set_filter_mag(FilterType::LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    this->unbind();

    stbi_image_free(data);
    return true;
}

/*----------------------------------------------------------------------------*/

bool Texture2D::do_reload() noexcept { return true; }

/*----------------------------------------------------------------------------*/

void Texture2D::do_unload() noexcept { glDeleteTextures(1, &m_texture_id); }

/*----------------------------------------------------------------------------*/
