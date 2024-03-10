#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class ShaderProgram
{
public:
    unsigned int ID;
    ShaderProgram(const std::string &vertexShaderAbsolutePath, const std::string &fragmentShaderAbsolutePath);
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void use();
};

#endif