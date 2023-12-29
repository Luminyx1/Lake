#include "Lake/SpriteComponent.h"

#include "Lake/PrimitiveShape.h"

#include <glad/glad.h>

lake::SpriteComponent::SpriteComponent(const std::string& texturePath, const Texture::FilterMode filterMode)
    : mMatrix(1.0f)
    , mTexture(texturePath, filterMode)
    , mShaderProgram("lake/assets/shaders/sprite.vsh", "lake/assets/shaders/sprite.fsh")
{ }

lake::SpriteComponent::SpriteComponent(const std::string& texturePath, const std::string& targetLayer, const Texture::FilterMode filterMode)
    : SpriteComponent(texturePath, filterMode)
{
    this->setTargetLayer(targetLayer);
}

lake::SpriteComponent::~SpriteComponent() = default;

void lake::SpriteComponent::draw(const RenderInfo& renderInfo) {
    mTexture.bind(0);

    mShaderProgram.bind();
    mShaderProgram.setMat4(0, mMatrix);
    mShaderProgram.setMat4(1, renderInfo.camera->getViewProjection());

    glBindVertexArray(PrimitiveShape::getQuadVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PrimitiveShape::getQuadEBO());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
