#include "program.hpp"

#include "utils/load_file.hpp"

#include <string>       // basic_string
#include <stdexcept>    // runtime_error
#include <type_traits>  // is_same
#include <vector>


frog::gl::shader::shader(GLenum type, const std::string& file)
        : _index(glCreateShader(type))
{
    static_assert(std::is_same_v<GLchar, char>,
            "Presumption that `GLchar` and `char` are the same type");

    auto content = load_file(file);
    if (content.empty())
    {
        throw std::runtime_error("shader: file '" + file + "' is empty");
    }
    const char* content_ptr = content.data();

    glShaderSource(_index, 1, &content_ptr, nullptr);
    glCompileShader(_index);

    GLint compiled = 0;
    glGetShaderiv(_index, GL_COMPILE_STATUS, &compiled);

    if (compiled == GL_FALSE)
    {
        GLint length = 0;
        glGetShaderiv(_index, GL_INFO_LOG_LENGTH, &length);

        auto buff = std::vector<GLchar>(length);
        glGetShaderInfoLog(_index, length, nullptr, buff.data());

        auto error_msg = std::string(buff.begin(), buff.end());

        // delete
        glDeleteShader(_index);
        _index = 0;

        throw std::runtime_error(
                "shader: compilation failed, file '" + file
                + "', error: " + error_msg);
    }
}


frog::gl::shader::~shader()
{
    glDeleteShader(_index);
}


void frog::gl::program::use() const
{
    glUseProgram(_program);
}


frog::gl::program::~program()
{
    glDeleteProgram(_program);
}


frog::gl::program::program(const std::string& vert_file
                         , const std::string& frag_file)
    : program(
        shader(GL_VERTEX_SHADER, vert_file),
        shader(GL_FRAGMENT_SHADER, frag_file)) {}

frog::gl::program::program(const shader& vert, const shader& frag)
    : _program(glCreateProgram())
{
    glAttachShader(_program, vert.index());
    glAttachShader(_program, frag.index());
    glLinkProgram(_program);
}


void frog::gl::program::uniform_mat(const geo::mat4& mat, GLint location)
{
    glProgramUniformMatrix4fv(index(), location, 1, true, mat.ptr());
}

void frog::gl::program::uniform_vec(const geo::vec4& vec, GLint location)
{
    glProgramUniform4fv(index(), location, 1, vec.ptr());
}

void frog::gl::program::uniform_vec(const geo::vec3& vec, GLint location)
{
    glProgramUniform3fv(index(), location, 1, vec.ptr());
}

void frog::gl::program::uniform_vec(const geo::vec2& vec, GLint location)
{
    glProgramUniform2fv(index(), location, 1, vec.ptr());
}
