#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:

    GLuint ID = 0;
    Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
    ~Shader();
    void use(void);
    void setUniMat4(std::string name, glm::mat4& data);
    void setUniFloat(std::string name, float data);



private:
    
    std::string readShaderCode(const std::string& filePath);
    GLuint compileShaderObject(GLenum type, std::string code);
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif