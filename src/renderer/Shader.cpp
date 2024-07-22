#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace GoL {

Shader::Shader(const Shader& other)
    : id(other.id)
    , uniformsCache(other.uniformsCache) {
}

Shader::Shader(const std::string& shaderString)
    : id(0) {
    ProgramSource source = ParseShaderStr(shaderString);
    id = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::Shader(std::istream& shaderStream)
    : id(0) {
    ProgramSource source = ParseShaderStream(shaderStream);
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

unsigned int Shader::GetShaderId() const {
    return id;
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

Shader::ProgramSource Shader::ParseShaderStream(std::istream& stream) {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line)) {
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

Shader::ProgramSource Shader::ParseShaderStr(const std::string& shaderStr) {
    auto vertexShIndex = shaderStr.find("#version", shaderStr.find("#shader vertex"));
    auto fragmentShIndex = shaderStr.find("#version", shaderStr.find("#shader fragment"));

    std::size_t i;
    return
    {
        shaderStr.substr(vertexShIndex, (i = shaderStr.find("#shader", vertexShIndex)) == std::string::npos ? shaderStr.size() : i - vertexShIndex),
        shaderStr.substr(fragmentShIndex, (i = shaderStr.find("#shader", fragmentShIndex)) == std::string::npos ? shaderStr.size() : i - fragmentShIndex)
    };
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
        std::cout << "Warning: uniform " << name << " not found" << std::endl;
    } else {
        uniformsCache[name] = location;
    }

    return location;
}

}
