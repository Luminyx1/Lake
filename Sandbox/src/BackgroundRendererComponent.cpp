#include "BackgroundRendererComponent.h"

#include "Lake/PrimitiveShape.h"
#include "Lake/Graphics.h"

BackgroundRendererComponent::BackgroundRendererComponent(const std::string& layerName)
    : DrawableComponent()
    , mWorkBuffer(lake::Graphics::getFramebufferSize())
    , mShaderProgram("lake/assets/shaders/compositor.vsh", "background.fsh")
    , mTime(0.0f)
{
    this->setTargetLayer(layerName);
    mWorkBuffer.addTextureBuffer(lake::Texture::Format::RGBA16F);
    mWorkBuffer.finalize();
}

void BackgroundRendererComponent::onEvent(lake::Event* event) {    
    if (event->getType() == lake::EventType::WindowResize) {
        lake::WindowResizeEvent* e = static_cast<lake::WindowResizeEvent*>(event);

        mWorkBuffer.resize(e->getSize());
    }
}

void BackgroundRendererComponent::draw(const lake::RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setFloat("uTime", mTime);
    mShaderProgram.setVec2("uResolution", glm::vec2(renderInfo.framebuffer->getSize()));
    mWorkBuffer.bind();
    renderInfo.framebuffer->getTextureBuffer(0)->bind(0);
    
    glBindVertexArray(lake::PrimitiveShape::getQuadVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lake::PrimitiveShape::getQuadEBO());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    lake::Framebuffer::blit(mWorkBuffer, *renderInfo.framebuffer, {0, 0}, mWorkBuffer.getSize(), {0, 0}, renderInfo.framebuffer->getSize(), static_cast<u32>(lake::Framebuffer::Type::Color));

    // Remember to re-bind the framebuffer after we are done.
    renderInfo.framebuffer->bind();
}

void BackgroundRendererComponent::update(f32 timeStep) {
    mTime += timeStep;
}
