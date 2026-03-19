#include <glad/glad.h>
#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{     
    // Read shader code
    std::string vertexCode = readShaderCode(vertexFilePath);
    std::string fragmentCode = readShaderCode(fragmentFilePath);

    // Compile objects
    GLuint vertexShader = compileShaderObject(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compileShaderObject(GL_FRAGMENT_SHADER, fragmentCode);
    
    ID = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    if (ID)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

void Shader::use(void)
{
    glUseProgram(ID);
}

std::string Shader::readShaderCode(const std::string& filePath)
{
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    
    shaderFile.open(filePath, std::ios::binary);
    shaderStream << shaderFile.rdbuf();

    return shaderStream.str();
}

GLuint Shader::compileShaderObject(GLenum type, std::string code)
{
    const char* CCode = code.c_str(); 
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &CCode, NULL); // provide code to object
    glCompileShader(shader); // compiles the objects code

    GLint success = GL_FALSE;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Shader compilation failed" << std::endl << infoLog << std::endl;
    }

    return shader;
}

GLuint Shader::createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "shaderProgram linking failed" << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void Shader::setUniMat4(std::string name, glm::mat4& data)
{
    GLuint location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::setUniFloat(std::string name, float data)
{
    GLuint location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, data);
}