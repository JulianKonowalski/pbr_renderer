#include "graphics/core/Shader.hpp"

#include "io/FileReader.hpp"

#include <glad/glad.h>
#include <iostream>

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

static constexpr unsigned int k_max_info_log_length = 512;

/*----------------------------------------------------------------------------*/

bool create_gl_shader(unsigned int& gl_shader_id,
                      const Shader::ShaderSourceFile& source_file) noexcept;

bool load_and_compile_gl_shader(
    const unsigned int gl_shader_id,
    const Shader::ShaderSourceFile& source_file) noexcept;

bool link_gl_program(const unsigned int gl_program_id,
                     const std::vector<unsigned int>& gl_shader_ids) noexcept;

/*----------------------------------------------------------------------------*/

unsigned int Shader::s_current_shader = GL_NONE;

/*----------------------------------------------------------------------------*/

Shader::Shader(const std::string& id,
               std::vector<ShaderSourceFile>& source_files)
    : vq::core::Resource(id), m_source_files(std::move(source_files)),
      m_shader_id(0) {}

/*----------------------------------------------------------------------------*/

void Shader::unbind_all() {
    glUseProgram(GL_NONE);
    Shader::s_current_shader = GL_NONE;
}

/*----------------------------------------------------------------------------*/

void Shader::bind() {
    glUseProgram(m_shader_id);
    Shader::s_current_shader = m_shader_id;
}

/*----------------------------------------------------------------------------*/

void Shader::unbind() {
    if (Shader::s_current_shader == m_shader_id) {
        glUseProgram(GL_NONE);
        Shader::s_current_shader = GL_NONE;
    }
}

/*----------------------------------------------------------------------------*/

bool Shader::do_load() noexcept {
    std::vector<unsigned int> gl_shader_ids(m_source_files.size(), 0);

    auto cleanup = [&]() {
        glDeleteProgram(m_shader_id);
        m_shader_id = 0;
        for (auto& gl_shader_id : gl_shader_ids) {
            glDeleteShader(gl_shader_id);
            gl_shader_id = 0;
        }
    };

    for (int i = 0; i < m_source_files.size(); ++i) {
        if (!create_gl_shader(gl_shader_ids[i], m_source_files[i]) ||
            !load_and_compile_gl_shader(gl_shader_ids[i], m_source_files[i])) {
            cleanup();
            return false;
        }
    }

    m_shader_id = glCreateProgram();
    if (!m_shader_id) {
        std::cout << "Failed to create an OpenGL program\n";
        cleanup();
        return false;
    }

    for (auto gl_shader_id : gl_shader_ids) {
        glAttachShader(m_shader_id, gl_shader_id);
    }

    if (!link_gl_program(m_shader_id, gl_shader_ids)) {
        cleanup();
        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------*/

bool Shader::do_reload() noexcept {
    int num_attached_shaders;
    glGetProgramiv(m_shader_id, GL_ATTACHED_SHADERS, &num_attached_shaders);

    std::vector<unsigned int> gl_shader_ids(num_attached_shaders, 0);
    glGetAttachedShaders(m_shader_id, num_attached_shaders, nullptr,
                         gl_shader_ids.data());

    for (int i = 0; i < m_source_files.size(); ++i) {
        if (!load_and_compile_gl_shader(gl_shader_ids[i], m_source_files[i])) {
            return false;
        }
    }

    if (!link_gl_program(m_shader_id, gl_shader_ids)) {
        return false;
    }

    return false;
}

/*----------------------------------------------------------------------------*/

void Shader::do_unload() noexcept {
    int num_attached_shaders;
    glGetProgramiv(m_shader_id, GL_ATTACHED_SHADERS, &num_attached_shaders);

    std::vector<unsigned int> gl_shader_ids(num_attached_shaders, 0);
    glGetAttachedShaders(m_shader_id, num_attached_shaders, nullptr,
                         gl_shader_ids.data());

    this->unbind();
    glDeleteProgram(m_shader_id);
    m_shader_id = 0;
    for (auto& gl_shader_id : gl_shader_ids) {
        glDeleteShader(gl_shader_id);
        gl_shader_id = 0;
    }
}

/*----------------------------------------------------------------------------*/

bool create_gl_shader(unsigned int& gl_shader_id,
                      const Shader::ShaderSourceFile& source_file) noexcept {
    gl_shader_id = 0;
    switch (source_file.type) {
    case Shader::ShaderSourceType::VERTEX:
        gl_shader_id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case Shader::ShaderSourceType::FRAGMENT:
        gl_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    if (!gl_shader_id) {
        std::cout << "Failed to create an OpenGL shader\n";
        return false;
    }
    return true;
}

bool load_and_compile_gl_shader(
    const unsigned int gl_shader_id,
    const Shader::ShaderSourceFile& source_file) noexcept {
    std::string source_code;
    vq::io::GLSLReader().read(source_code, source_file.source_file_path);
    if (source_code.length() <= 0) {
        std::cout << "Failed to read a shader source file: "
                  << source_file.source_file_path << "\n";
        return false;
    }

    int success;
    const char* source_code_cstring = source_code.c_str();
    glShaderSource(gl_shader_id, 1, &source_code_cstring, nullptr);
    glCompileShader(gl_shader_id);
    glGetShaderiv(gl_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[k_max_info_log_length];
        glGetShaderInfoLog(gl_shader_id, k_max_info_log_length, nullptr,
                           info_log);
        std::cout << "Failed to compile shader source file "
                  << source_file.source_file_path
                  << "\nCompilation finished with errors:\n"
                  << info_log << "\n";
        return false;
    }

    return true;
}

bool link_gl_program(const unsigned int gl_program_id,
                     const std::vector<unsigned int>& gl_shader_ids) noexcept {
    int success;
    glLinkProgram(gl_program_id);
    glGetProgramiv(gl_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[k_max_info_log_length];
        glGetShaderInfoLog(gl_program_id, k_max_info_log_length, nullptr,
                           info_log);
        std::cout << "Failed to link shaders. Linking finished with errors:\n"
                  << info_log << "\n";
        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------*/
