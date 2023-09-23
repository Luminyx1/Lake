#include "Lake/ShaderProgram.h"

#include "Lake/Log.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

std::unordered_map<std::string, std::string> lake::ShaderProgram::Shader::sourceCache;

lake::ShaderProgram::Shader::Shader(const std::string& path, const GLenum type)
    : mID(GL_NONE)
{
    const auto compile = [path, type](const std::string& code) -> u32 {
        lake::trace("Compiling shader: ", path.c_str());

        const u32 id = glCreateShader(type);
        const char* src = code.c_str();

        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        i32 success = false;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) [[unlikely]] {
            i32 length = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> error(length);
            glGetShaderInfoLog(id, length, &length, error.data());

            LK_ASSERT(false, "Shader compilation failed: ", error.data());
        }

        return id;
    };

    for (const auto& [cachedPath, cachedSource] : sourceCache) {
        if (cachedPath == path) {
            mID = compile(cachedSource);
            return;
        }
    }

    std::ifstream file(path);
    LK_ASSERT(file.is_open(), "Failed to open shader file: ", path.c_str());

    const std::string shader(std::istreambuf_iterator<char>{file}, {});
    mID = compile(shader);

    sourceCache[path] = shader;
}

lake::ShaderProgram::Shader::~Shader() {
    glDeleteShader(mID);
}

lake::ShaderProgram::ShaderProgram(const std::string& vshPath, const std::string& fshPath)
    : mID(glCreateProgram())
{
    const Shader vsh(vshPath, GL_VERTEX_SHADER);
    const Shader fsh(fshPath, GL_FRAGMENT_SHADER);

    { // Link shaders
        glAttachShader(mID, vsh.getID());
        glAttachShader(mID, fsh.getID());
        
        glLinkProgram(mID);

        i32 success = false;
        glGetProgramiv(mID, GL_LINK_STATUS, &success);
        if (!success) {
            i32 length = 0;
            glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> error(length);
            glGetProgramInfoLog(mID, length, &length, error.data());

            LK_ASSERT(false, "Shader program linking failed: ", error.data());
        }

        glDetachShader(mID, vsh.getID());
        glDetachShader(mID, fsh.getID());

    }

    glValidateProgram(mID);

    { // Cache uniform locations
        i32 count = 0;
        glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &count);

        i32 maxLength = 0;
        glGetProgramiv(mID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

        if (count == 0)
            return;

        std::vector<char> name(maxLength);

        for (std::int_fast16_t i = 0; i < count; ++i) {
            u32 data; i32 length, size;
            glGetActiveUniform(mID, i, maxLength, &length, &size, &data, name.data());

            mUniformLocations.emplace(name.data(), glGetUniformLocation(mID, name.data()));
        }
    }
}

lake::ShaderProgram::~ShaderProgram() {
    glDeleteProgram(mID);
}

void lake::ShaderProgram::bind() const {
    glUseProgram(mID);
}

void lake::ShaderProgram::setInt(const std::string& name, const i32 value) const {
    this->setInt(this->getLocation(name), value);
}

void lake::ShaderProgram::setFloat(const std::string& name, const f32 value) const {
    this->setFloat(this->getLocation(name), value);
}

void lake::ShaderProgram::setDouble(const std::string& name, const f64 value) const {
    this->setDouble(this->getLocation(name), value);
}

void lake::ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    this->setVec2(this->getLocation(name), value);
}

void lake::ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    this->setVec3(this->getLocation(name), value);
}

void lake::ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    this->setVec4(this->getLocation(name), value);
}

void lake::ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const {
    this->setMat4(this->getLocation(name), value);
}

void lake::ShaderProgram::setInt(const i32 location, const i32 value) const {
    glUniform1i(location, value);
}

void lake::ShaderProgram::setFloat(const i32 location, const f32 value) const {
    glUniform1f(location, value);
}

void lake::ShaderProgram::setDouble(const i32 location, const f64 value) const {
    glUniform1d(location, value);
}

void lake::ShaderProgram::setVec2(const i32 location, const glm::vec2& value) const {
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void lake::ShaderProgram::setVec3(const i32 location, const glm::vec3& value) const {
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void lake::ShaderProgram::setVec4(const i32 location, const glm::vec4& value) const {
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void lake::ShaderProgram::setMat4(const i32 location, const glm::mat4& value) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void lake::ShaderProgram::setOptionalInt(const std::string& name, const i32 value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setInt(location, value);
    }
}

void lake::ShaderProgram::setOptionalFloat(const std::string& name, const f32 value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setFloat(location, value);
    }
}

void lake::ShaderProgram::setOptionalDouble(const std::string& name, const f64 value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setDouble(location, value);
    }
}

void lake::ShaderProgram::setOptionalVec2(const std::string& name, const glm::vec2& value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setVec2(location, value);
    }
}

void lake::ShaderProgram::setOptionalVec3(const std::string& name, const glm::vec3& value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setVec3(location, value);
    }
}

void lake::ShaderProgram::setOptionalVec4(const std::string& name, const glm::vec4& value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setVec4(location, value);
    }
}

void lake::ShaderProgram::setOptionalMat4(const std::string& name, const glm::mat4& value) const {
    const i32 location = this->getLocation(name);
    if (location != -1) {
        this->setMat4(location, value);
    }
}

i32 lake::ShaderProgram::getLocation(const std::string& name) const {
    const auto it = mUniformLocations.find(name);
    if (it != mUniformLocations.end()) {
        return it->second;
    }

    return -1;
}

void lake::ShaderProgram::clearCache() {
    Shader::sourceCache.clear();
}
