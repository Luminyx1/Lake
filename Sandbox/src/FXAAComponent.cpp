#include "FXAAComponent.h"

#include "Lake/PrimitiveShape.h"
#include "Lake/Graphics.h"

FXAAComponent::FXAAComponent(const std::string& layerName)
    : DrawableComponent()
    , mWorkBuffer(lake::Graphics::getFramebufferSize())
    , mShaderProgram("lake/assets/shaders/compositor.vsh", "fxaa.fsh")
{
    this->setTargetLayer(layerName);
    mWorkBuffer.addTextureBuffer(lake::Texture::Format::RGBA16F);
    mWorkBuffer.finalize();
}

void FXAAComponent::draw(const lake::RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setVec2(0, {
        1.0f / static_cast<f32>(mWorkBuffer.getSize().x),
        1.0f / static_cast<f32>(mWorkBuffer.getSize().y)
    });

    mWorkBuffer.bind();
    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);
    
    glBindVertexArray(lake::PrimitiveShape::getQuadVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lake::PrimitiveShape::getQuadEBO());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lake::Framebuffer::blit(mWorkBuffer, *renderInfo.framebuffer, {0, 0}, mWorkBuffer.getSize(), {0, 0}, renderInfo.framebuffer->getSize(), static_cast<u32>(lake::Framebuffer::Type::Color));
}

void FXAAComponent::resize(const glm::u32vec2& size) {
    mWorkBuffer.resize(size);
}
