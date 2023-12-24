#include "Lake/Framebuffer.h"

#include "Lake/Log.h"

#include <glm/gtc/type_ptr.hpp>

lake::Framebuffer::Framebuffer()
    : mID(GL_NONE)
    , mSize({ 0, 0 })
    , mTextureBuffers()
    , mDepthStencil(nullptr)
    , mFinalized(true)
{ }

lake::Framebuffer::Framebuffer(const glm::u32vec2& size)
    : mID(GL_NONE)
    , mSize(size)
    , mTextureBuffers()
    , mDepthStencil(nullptr)
    , mFinalized(false)
{
    glCreateFramebuffers(1, &mID);
    LK_ASSERT(mID != GL_NONE, "Failed to create framebuffer!");

    mTextureBuffers.reserve(8);
}

lake::Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &mID);

    for (auto& textureBuffer : mTextureBuffers) {
        delete textureBuffer;
    }

    delete mDepthStencil;
}

void lake::Framebuffer::bind(const BindMode mode) const {
    switch (mode) {
        case BindMode::Draw:  glBindFramebuffer(GL_FRAMEBUFFER, mID); break;
        case BindMode::Read:  glBindFramebuffer(GL_READ_FRAMEBUFFER, mID); break;
        case BindMode::Write: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mID); break;
    }
}

void lake::Framebuffer::clear(const glm::f32vec4& value, const Type type, const u32 drawBuffer) const {
    if (type & Type::Color) {
        glClearNamedFramebufferfv(mID, GL_COLOR, drawBuffer, glm::value_ptr(value));
    }

    if (type & Type::Depth) {
        glClearNamedFramebufferfv(mID, GL_DEPTH, 0, glm::value_ptr(value));
    }
}

void lake::Framebuffer::clear(const glm::u32vec4& value, const Type type, const u32 drawBuffer) const {
    if (type & Type::Color) {
        glClearNamedFramebufferuiv(mID, GL_COLOR, drawBuffer, glm::value_ptr(value));
    }

    if (type & Type::Depth) {
        glClearNamedFramebufferuiv(mID, GL_DEPTH, 0, glm::value_ptr(value));
    }
}

void lake::Framebuffer::resize(const glm::u32vec2& size) {
    LK_ASSERT(mID != GL_NONE, "Cannot resize backbuffer!");

    mSize = size;

    // as for the texture buffers, we can't resize them so we have to delete them and recreate them

    std::vector<Texture*> newTextureBuffers;
    newTextureBuffers.reserve(mTextureBuffers.size());

    for (auto& textureBuffer : mTextureBuffers) {
        newTextureBuffers.push_back(new Texture(size, textureBuffer->getFormat(), textureBuffer->getFilterMode()));
        glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0 + static_cast<u32>(newTextureBuffers.size()) - 1, newTextureBuffers.back()->getID(), 0);
        delete textureBuffer;
    }

    mTextureBuffers = std::move(newTextureBuffers);

    if (mDepthStencil != nullptr) {
        Texture* newDepthBuffer = new Texture(size, mDepthStencil->getFormat(), mDepthStencil->getFilterMode());
        glNamedFramebufferTexture(mID, GL_DEPTH_STENCIL_ATTACHMENT, newDepthBuffer->getID(), 0);
        delete mDepthStencil;
        mDepthStencil = newDepthBuffer;
    }
}

const lake::Framebuffer* lake::Framebuffer::getBackbuffer() {
    static const Framebuffer backbuffer = Framebuffer();
    return &backbuffer;
}

void lake::Framebuffer::blit(const Framebuffer& src, const Framebuffer& dst, const glm::u32vec2& srcStart, const glm::u32vec2& srcEnd, const glm::u32vec2& dstStart, const glm::u32vec2& dstEnd, const u32 typeMask, const Texture::FilterMode filterMode) {
    u32 mask = 0;
    if (typeMask & static_cast<u32>(Type::Color)) {
        mask |= GL_COLOR_BUFFER_BIT;
    }
    if (typeMask & static_cast<u32>(Type::Depth)) {
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    glBlitNamedFramebuffer(src.getID(), dst.getID(), srcStart.x, srcStart.y, srcEnd.x, srcEnd.y, dstStart.x, dstStart.y, dstEnd.x, dstEnd.y, mask, static_cast<GLenum>(filterMode));
}

void lake::Framebuffer::addTextureBuffer(const Texture::Format fmt, const Texture::FilterMode enlargeFilter, const Texture::FilterMode shrinkFilter) {
    LK_ASSERT(mID != GL_NONE, "Cannot add texture buffer to backbuffer!");
    LK_ASSERT(mFinalized == false, "Cannot add texture buffer to finalized framebuffer!");

    if (fmt == Texture::Format::Depth24Stencil8 || fmt == Texture::Format::Depth32FStencil8) {
        LK_ASSERT(mDepthStencil == nullptr, "Framebuffer already has depth/stencil attachment!");
        mDepthStencil = new Texture(mSize, fmt, enlargeFilter);
        glNamedFramebufferTexture(mID, GL_DEPTH_STENCIL_ATTACHMENT, mDepthStencil->getID(), 0);
    } else {
        mTextureBuffers.push_back(new Texture(mSize, fmt, enlargeFilter));
        glNamedFramebufferTexture(mID, GL_COLOR_ATTACHMENT0 + static_cast<u32>(mTextureBuffers.size()) - 1, mTextureBuffers.back()->getID(), 0);
    }
}

void lake::Framebuffer::finalize() const {
    LK_ASSERT(mID != GL_NONE, "Cannot finalize backbuffer!");

    u32 attachments[32] = { 0 };

    for (std::uint_fast8_t i = 0; i < mTextureBuffers.size(); i++) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glNamedFramebufferDrawBuffers(mID, static_cast<GLsizei>(mTextureBuffers.size()), attachments);

    u32 status = glCheckNamedFramebufferStatus(mID, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char* statusStr = "Unknown";

        switch (status) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: statusStr = "Not all framebuffer attachments are complete."; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: statusStr = "Framebuffer has no attachments."; break;
        }

        LK_ASSERT(false, "Framebuffer incomplete: ", statusStr);
    }
}
