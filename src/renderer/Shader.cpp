#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace GoL {

Shader::Shader(const std::string& shaderFilePath)
    : id(0) {
    ProgramSource source = ParseShader(shaderFilePath);
    id = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::Shader(const std::string& shaderVertexFilePath, const std::string& shaderFragmentFilePath)
    : id(0) {
    std::string vertexSource = ParseFile(shaderVertexFilePath);
    std::string fragmentSource = ParseFile(shaderFragmentFilePath);
    id = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::Bind() const {
    glUseProgram(id);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4) {
    glUniform4f(GetUniformLocation(name), f1, f2, f3, f4);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

std::string Shader::ParseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::string str;
    std::string content;
    while (std::getline(file, str)) {
        content.append(str + "\n");
    }
    return content;
}

Shader::ProgramSource Shader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else {
                type = ShaderType::FRAGMENT;
            }
        } else {
            stringStream[(int) type] << line << '\n';
        }
    }

    return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& name) {
    unsigned int id = glCreateShader(type);
    const char* src = name.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
    if (uniformsCache.find(name) != uniformsCache.end()) {
        return uniformsCache[name];
    }

    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cout << "Warinng: uniform " << name << " not found" << std::endl;
    } else {
        uniformsCache[name] = location;
    }

    return location;
}

}
