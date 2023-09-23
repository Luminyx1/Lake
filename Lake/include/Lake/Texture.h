#pragma once

#include "Lake/Common.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace lake {

    class Texture {
    public:
        enum class FilterMode {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,

            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,

            Count
        };

    private:
        Texture();

    public:
        Texture(const std::string& path, const FilterMode filterMode = FilterMode::Linear);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept
            : mID(other.mID)
            , mSize(other.mSize)
        {
            other.mID = GL_NONE;
        }

        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {
                mID = other.mID;
                mSize = other.mSize;

                other.mID = GL_NONE;
            }

            return *this;
        }

        void initFromFile(const std::string& path, const FilterMode filterMode = FilterMode::Linear);
        void initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, const FilterMode filterMode = FilterMode::Linear);

        void bind(const u32 slot) const;

        [[nodiscard]] const glm::vec2& getSize() const { return mSize; }

        static void clearCache();

    private:
        static std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> cache;

        u32 mID;
        glm::u32vec2 mSize;
    };

} // namespace lake
