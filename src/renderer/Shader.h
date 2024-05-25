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
    Shader(const std::string& shaderFilePath);
    Shader(const std::string& shaderVertexFilePath, const std::string& shaderFragmentFilePath);
    ~Shader();

    void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    unsigned int GetShaderId() const;

    void Bind() const;
    void Unbind() const;

private:
    ProgramSource ParseShader(const std::string& filePath);
    std::string ParseFile(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& name);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name);
};

}
