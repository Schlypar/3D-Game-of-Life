#pragma once

#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>

#include <string>
#include <unordered_map>

namespace GoL {

class Shader {
    struct ProgramSource {
        std::string vertexSource;
        std::string fragmentSource;
    };

private:
    unsigned int id;
    std::unordered_map<std::string, int> uniformsCache;

public:
    Shader(const Shader& other);
    Shader(const std::string& shaderString);
    Shader(std::istream& shaderStream);
    Shader(const std::string& shaderVertexFilePath, const std::string& shaderFragmentFilePath);
    ~Shader();

    /**
    * @name SetUniform4f - Uploads vec4 to the GPU program
    * @param name - Name of uniform variable
    * @param f1 - Value of first coordinate
    * @param f2 - Value of second coordinate
    * @param f3 - Value of third coordinate
    * @param f4 - Value of fourth coordinate
    * @return void
    */
    void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);

    /**
    * @name SetUniformMat4f - Uploads mat4x4 to the GPU program
    * @param name - Name of uniform variable
    * @param matrix -  matrix to be uploaded
    * @return void
    */
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    /**
    * @name GetShaderId - Returns id of shader
    * @return id of shader
    */
    unsigned int GetShaderId() const;

    /**
    * @name Bind - Tells OpenGL to use this shader
    * @return void
    */
    void Bind() const;

    /**
    * @name Unbind - Tells OpenGL to not use any shader
    * @return void
    */
    void Unbind() const;

private:
    ProgramSource ParseShaderStream(std::istream& filePath);
    ProgramSource ParseShaderStr(const std::string& filePath);
    std::string ParseFile(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& name);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name);
};

}
