#include "basicShader.h"

#include <iostream>

namespace Renderer {
    basicShader::basicShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "VERTEX SHADER compile-time error" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(programID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
        }
        else
        {
            vIsCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }


    bool basicShader::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    basicShader::~basicShader()
    {
        glDeleteProgram(programID);
    }

    void basicShader::use() const
    {
        glUseProgram(programID);
    }

    basicShader& basicShader::operator=(basicShader&& basicShader) noexcept
    {
        glDeleteProgram(programID);
        programID = basicShader.programID;
        vIsCompiled = basicShader.vIsCompiled;

        basicShader.programID = 0;
        basicShader.vIsCompiled = false;
        return *this;
    }

    basicShader::basicShader(basicShader&& basicShader) noexcept
    {
        programID = basicShader.programID;
        vIsCompiled = basicShader.vIsCompiled;

        basicShader.programID = 0;
        basicShader.vIsCompiled = false;
    }
}