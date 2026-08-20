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

void get_attached_gl_shaders(const unsigned int gl_program_id,
                             std::vector<unsigned int>& gl_shader_ids) noexcept;

void cleanup_gl_shaders(unsigned int& gl_program_id,
                        std::vector<unsigned int>& gl_shader_ids) noexcept;

/*----------------------------------------------------------------------------*/

Shader::Shader(const std::string& id,
               std::vector<ShaderSourceFile>& source_files)
    : vq::core::Resource(id), m_source_files(std::move(source_files)),
      m_shader_id(0) {}

/*----------------------------------------------------------------------------*/

Shader::Shader(Shader&& other) : vq::core::Resource("") {
    *this = std::move(other);
}

/*----------------------------------------------------------------------------*/

Shader& Shader::operator=(Shader&& other) {
    m_source_files    = std::move(other.m_source_files);
    m_shader_id       = other.m_shader_id;
    other.m_shader_id = 0;
    vq::core::Resource::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

void Shader::unbind_all() { glUseProgram(GL_NONE); }

/*----------------------------------------------------------------------------*/

void Shader::bind() { glUseProgram(m_shader_id); }

/*----------------------------------------------------------------------------*/

void Shader::unbind() {
    int current_gl_program_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_gl_program_id);
    if (current_gl_program_id != m_shader_id) {
        return;
    }
    glUseProgram(GL_NONE);
}

/*----------------------------------------------------------------------------*/

bool Shader::do_load() noexcept {
    std::vector<unsigned int> gl_shader_ids(m_source_files.size(), 0);

    for (int i = 0; i < m_source_files.size(); ++i) {
        if (!create_gl_shader(gl_shader_ids[i], m_source_files[i]) ||
            !load_and_compile_gl_shader(gl_shader_ids[i], m_source_files[i])) {
            cleanup_gl_shaders(m_shader_id, gl_shader_ids);
            return false;
        }
    }

    m_shader_id = glCreateProgram();
    if (!m_shader_id) {
        std::cout << "Failed to create an OpenGL program\n";
        cleanup_gl_shaders(m_shader_id, gl_shader_ids);
        return false;
    }

    for (auto gl_shader_id : gl_shader_ids) {
        glAttachShader(m_shader_id, gl_shader_id);
    }

    if (!link_gl_program(m_shader_id, gl_shader_ids)) {
        cleanup_gl_shaders(m_shader_id, gl_shader_ids);
        return false;
    }

    return true;
}

/*----------------------------------------------------------------------------*/

bool Shader::do_reload() noexcept {
    std::vector<unsigned int> gl_shader_ids;
    get_attached_gl_shaders(m_shader_id, gl_shader_ids);

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
    std::vector<unsigned int> gl_shader_ids;
    get_attached_gl_shaders(m_shader_id, gl_shader_ids);

    this->unbind();
    cleanup_gl_shaders(m_shader_id, gl_shader_ids);
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

void get_attached_gl_shaders(
    const unsigned int gl_program_id,
    std::vector<unsigned int>& gl_shader_ids) noexcept {
    int num_attached_shaders;
    glGetProgramiv(gl_program_id, GL_ATTACHED_SHADERS, &num_attached_shaders);
    gl_shader_ids.resize(num_attached_shaders);
    glGetAttachedShaders(gl_program_id, num_attached_shaders, nullptr,
                         gl_shader_ids.data());
}

void cleanup_gl_shaders(unsigned int& gl_program_id,
                        std::vector<unsigned int>& gl_shader_ids) noexcept {
    glDeleteProgram(gl_program_id);
    gl_program_id = 0;
    for (auto& gl_shader_id : gl_shader_ids) {
        glDeleteShader(gl_shader_id);
        gl_shader_id = 0;
    }
}

/*----------------------------------------------------------------------------*/
