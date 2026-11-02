#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
    GLuint id = 0;

    Shader(const std::string& vertPath, const std::string& fragPath) {
        GLuint vs = compile(GL_VERTEX_SHADER, readFile(vertPath), vertPath);
        GLuint fs = compile(GL_FRAGMENT_SHADER, readFile(fragPath), fragPath);

        id = glCreateProgram();
        glAttachShader(id, vs);
        glAttachShader(id, fs);
        glLinkProgram(id);

        GLint ok;
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetProgramInfoLog(id, 1024, nullptr, log);
            std::cerr << "Linkovanje shadera nije uspelo:\n" << log << "\n";
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    ~Shader() { glDeleteProgram(id); }

    void use() const { glUseProgram(id); }

    void setMat4(const std::string& name, const glm::mat4& m) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    }
    void setVec3(const std::string& name, const glm::vec3& v) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
    }

private:
    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file) std::cerr << "Ne mogu da otvorim: " << path << "\n";
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    static GLuint compile(GLenum type, const std::string& src, const std::string& name) {
        GLuint s = glCreateShader(type);
        const char* c = src.c_str();
        glShaderSource(s, 1, &c, nullptr);
        glCompileShader(s);

        GLint ok;
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        
        if (!ok) {
            char log[1024];
            glGetShaderInfoLog(s, 1024, nullptr, log);
            std::cerr << "Greška u shaderu " << name << ":\n" << log << "\n";
        }

        return s;
    }
};