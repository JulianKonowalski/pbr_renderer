#include "graphics/core/Shader.hpp"

#include "io/FileReader.hpp"

#include <glad/glad.h>
#include <stdexcept>

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

Shader::Shader(const std::string& id, const std::string& asset_path,
               const ShaderType shader_type)
    : vq::core::Resource(id, asset_path), m_shader_id(0) {
    switch (shader_type) {
    case ShaderType::VERTEX:
        m_shader_id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case ShaderType::FRAGMENT:
        m_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    if (!m_shader_id) {
        throw std::runtime_error("Failed to create shader " + id +
                                 " during a call to " + __FUNCTION__);
    }
}

/*----------------------------------------------------------------------------*/

Shader::~Shader() { glDeleteShader(m_shader_id); }

/*----------------------------------------------------------------------------*/

Shader::Shader(Shader&& other) : vq::core::Resource("", "") {
    *this = std::move(other);
}

/*----------------------------------------------------------------------------*/

Shader& Shader::operator=(Shader&& other) {
    m_shader_id       = other.m_shader_id;
    other.m_shader_id = 0;
    vq::core::Resource::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

#include <iostream>

bool Shader::do_load() noexcept {
    std::string source;
    vq::io::GLSLReader().read(source, this->get_asset_path());

    if (source.length() <= 0) {
        /** TODO: Create a logging interface */
        std::cout << "Failed to load shader source file: "
                  << this->get_asset_path() << std::endl;
        return false;
    }

    const char* source_string = source.c_str();

    glShaderSource(m_shader_id, 1, &source_string, nullptr);
    glCompileShader(m_shader_id);

    int success;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(m_shader_id, 512, nullptr, info_log);

        /** TODO: Create a logging interface */
        std::cout << "Failed to compile shader " << this->get_id() << ":\n"
                  << info_log << std::endl;

        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------*/

void Shader::do_unload() noexcept {}

/*----------------------------------------------------------------------------*/
