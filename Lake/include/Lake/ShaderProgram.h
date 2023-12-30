#pragma once

#include "Lake/Common.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace lake {

    class ShaderProgram final {
        LK_NO_COPY(ShaderProgram);

    private:
        class Shader {
        public:
            Shader(const std::string& path, const GLenum type);
            ~Shader();

            [[nodiscard]] u32 getID() const { return mID; }

            static std::unordered_map<std::string, std::string> sSourceCache;

        private:
            u32 mID;
        };

    public:
        ShaderProgram(const std::string& vshPath, const std::string& fshPath);
        ~ShaderProgram();

        ShaderProgram(ShaderProgram&& other) noexcept
            : mID(other.mID)
            , mUniformLocations(std::move(other.mUniformLocations))
        {
            other.mID = GL_NONE;
        }

        ShaderProgram& operator=(ShaderProgram&& other) noexcept {
            if (this != &other) {
                mID = other.mID;
                other.mID = GL_NONE;

                mUniformLocations = std::move(other.mUniformLocations);
            }

            return *this;
        }
        
        void bind() const;

        void setInt(const std::string& name, const i32 value) const;
        void setFloat(const std::string& name, const f32 value) const;
        void setDouble(const std::string& name, const f64 value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        void setInt(const i32 location, const i32 value) const;
        void setFloat(const i32 location, const f32 value) const;
        void setDouble(const i32 location, const f64 value) const;
        void setVec2(const i32 location, const glm::vec2& value) const;
        void setVec3(const i32 location, const glm::vec3& value) const;
        void setVec4(const i32 location, const glm::vec4& value) const;
        void setMat4(const i32 location, const glm::mat4& value) const;

        void setOptionalInt(const std::string& name, const i32 value) const;
        void setOptionalFloat(const std::string& name, const f32 value) const;
        void setOptionalDouble(const std::string& name, const f64 value) const;
        void setOptionalVec2(const std::string& name, const glm::vec2& value) const;
        void setOptionalVec3(const std::string& name, const glm::vec3& value) const;
        void setOptionalVec4(const std::string& name, const glm::vec4& value) const;
        void setOptionalMat4(const std::string& name, const glm::mat4& value) const;

        static void clearCache();

    private:
        i32 getLocation(const std::string& name) const;

        std::unordered_map<std::string, i32> mUniformLocations;
        u32 mID;
    };

} // namespace lake
