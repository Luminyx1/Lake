#include "Lake/Texture.h"

#include "Lake/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <unordered_map>

std::unordered_map<std::string, std::tuple<u8*, glm::u32vec2, u32>> lake::Texture::cache;

lake::Texture::Texture()
    : mID(GL_NONE)
    , mSize(0, 0)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mID);
}

lake::Texture::Texture(const std::string& path, const FilterMode filterMode)
    : Texture()
{
    this->initFromFile(path, filterMode);
}

lake::Texture::~Texture() {
    if (mID != GL_NONE) [[likely]] {
        glDeleteTextures(1, &mID);
    }
}

void lake::Texture::initFromFile(const std::string& path, const FilterMode filterMode) {
    LK_ASSERT(mID != GL_NONE, "Cannot initialize invalid texture");

    const auto load = [&path]() -> std::tuple<u8*, glm::u32vec2, u32> {
        for (const auto& [cachedPath, cachedData] : lake::Texture::cache) {
            if (cachedPath == path) {
                return cachedData;
            }
        }

        i32 width = 0, height = 0, channels = 0;

        stbi_set_flip_vertically_on_load(true);
        u8* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        LK_ASSERT(data != nullptr, "Failed to load texture from file: ", path);

        lake::Texture::cache.emplace(path, std::make_tuple(data, glm::u32vec2(width, height), static_cast<u32>(channels)));

        return { data, { static_cast<u32>(width), static_cast<u32>(height) }, static_cast<u32>(channels) };
    };

    const auto& [data, size, channels] = load();

    this->initFromData(data, channels, size, filterMode);

    lake::info("Loaded texture from file: ", path);
}

void lake::Texture::initFromData(const u8* data, const u32 channelCount, const glm::u32vec2& size, const FilterMode filterMode) {
    LK_ASSERT(mID != GL_NONE, "Cannot initialize invalid texture");

    mSize = size;

    glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filterMode));
    glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filterMode));

    const auto safeSubImage = [this, data, size](const u32 format) {
        if (data == nullptr) [[unlikely]] {
            return;
        }

        glTextureSubImage2D(mID, 0, 0, 0, size.x, size.y, format, GL_UNSIGNED_BYTE, data);
    };

    if (channelCount == 4) {
        glTextureStorage2D(mID, 1, GL_RGBA8, size.x, size.y);
        safeSubImage(GL_RGBA);
    } else if (channelCount == 3) {
        glTextureStorage2D(mID, 1, GL_RGB8, size.x, size.y);
        safeSubImage(GL_RGB);
    } else if (channelCount == 1) {
        glTextureStorage2D(mID, 1, GL_R8, size.x, size.y);
        safeSubImage(GL_RED);
    } else [[unlikely]] {
        LK_ASSERT(false, "Unknown texture channel count: ", channelCount);
    }

    glTextureParameteri(mID, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(mID, GL_TEXTURE_MAX_LEVEL, 6);
    glGenerateTextureMipmap(mID);
}

void lake::Texture::bind(const u32 slot) const {
    LK_ASSERT(mID != GL_NONE, "Cannot bind invalid texture");

    glBindTextureUnit(slot, mID);
}

void lake::Texture::clearCache() {
    for (const auto& [path, data] : lake::Texture::cache) {
        const auto& [dataPtr, size, channels] = data;

        stbi_image_free(dataPtr);
    }

    lake::Texture::cache.clear();
}
