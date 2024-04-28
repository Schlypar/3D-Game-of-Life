#pragma once

#include <glad/gl.h>
#include <string>
#include <unordered_map>

class Shader {
    struct ProgramSource {
        std::string vertexSource;
        std::string fragmentSource;
    };

private:
    unsigned int id;
    std::unordered_map<std::string, int> uniformsCache;

public:
    Shader(const std::string& shaderFilePath);
    Shader(const std::string& shaderVertexFilePath, const std::string& shaderFragmentFilePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

private:
    ProgramSource ParseShader(const std::string& filePath);
    std::string ParseFile(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& name);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name);
};