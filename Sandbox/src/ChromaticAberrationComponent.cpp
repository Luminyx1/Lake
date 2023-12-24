#include "ChromaticAberrationComponent.h"

#include "Lake/PrimitiveShape.h"
#include "Lake/Graphics.h"

ChromaticAberrationComponent::ChromaticAberrationComponent(const std::string& layerName)
    : DrawableComponent()
    , mWorkBuffer(lake::Graphics::getFramebufferSize())
    , mShaderProgram("lake/assets/shaders/compositor.vsh", "chromatic_aberration.fsh")
{
    this->setTargetLayer(layerName);
    mWorkBuffer.addTextureBuffer(lake::Texture::Format::RGBA16F);
    mWorkBuffer.finalize();
}

void ChromaticAberrationComponent::draw(const lake::RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mWorkBuffer.bind();
    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);
    
    glBindVertexArray(lake::PrimitiveShape::getQuadVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lake::PrimitiveShape::getQuadEBO());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lake::Framebuffer::blit(mWorkBuffer, *renderInfo.framebuffer, {0, 0}, mWorkBuffer.getSize(), {0, 0}, renderInfo.framebuffer->getSize(), static_cast<u32>(lake::Framebuffer::Type::Color));

    // Remember to re-bind the framebuffer after we are done.
    renderInfo.framebuffer->bind();
}

void ChromaticAberrationComponent::resize(const glm::u32vec2& size) {
    mWorkBuffer.resize(size);
}
