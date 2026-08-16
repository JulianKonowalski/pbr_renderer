#include "graphics/core/Program.hpp"

#include <glad/glad.h>
#include <iostream>
#include <stdexcept>

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

unsigned int Program::s_current_program = 0;

/*----------------------------------------------------------------------------*/

Program::Program(const std::string& id, const std::vector<Shader*>& shaders)
    : vq::core::Resource(id, ""), m_shaders(shaders), m_program_id(0) {
    m_program_id = glCreateProgram();
    if (!m_program_id) {
        throw std::runtime_error("Failed to create program " + id +
                                 " during a call to " + __FUNCTION__);
    }
}

/*----------------------------------------------------------------------------*/

Program::~Program() {
    this->unbind();
    glDeleteProgram(m_program_id);
}

/*----------------------------------------------------------------------------*/

void Program::unbind_all() {
    glUseProgram(GL_NONE);
    Program::s_current_program = 0;
}

/*----------------------------------------------------------------------------*/

void Program::bind() {
    glUseProgram(m_program_id);
    Program::s_current_program = 0;
}

/*----------------------------------------------------------------------------*/

void Program::unbind() {
    if (s_current_program == m_program_id) {
        glUseProgram(GL_NONE);
        Program::s_current_program = GL_NONE;
    }
}

/*----------------------------------------------------------------------------*/

bool Program::do_load() noexcept {
    for (const auto* shader : m_shaders) {
        glAttachShader(m_program_id, shader->m_shader_id);
    }

    glLinkProgram(m_program_id);

    int success;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(m_program_id, 512, nullptr, info_log);

        /** TODO: Create a logging interface */
        std::cout << "Failed to link program " << this->get_id() << ":\n"
                  << info_log << std::endl;

        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------*/

bool Program::do_reload() noexcept { return false; }

/*----------------------------------------------------------------------------*/

void Program::do_unload() noexcept {}

/*----------------------------------------------------------------------------*/
