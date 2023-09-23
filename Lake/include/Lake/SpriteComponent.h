#pragma once

#include "Lake/Common.h"

#include "Lake/DrawableComponent.h"
#include "Lake/Texture.h"
#include "Lake/ShaderProgram.h"

namespace lake {

    class SpriteComponent : public DrawableComponent {
    public:
        SpriteComponent(const std::string& texturePath);
        ~SpriteComponent() override;

        void draw(const RenderInfo& renderInfo) override;

        void setMatrix(const glm::mat4& matrix) { mMatrix = matrix; }
        [[nodiscard]] const glm::mat4& getMatrix() const { return mMatrix; }
        
        void setTargetLayer(const std::string& layerName) { mTargetLayerHash = std::hash<std::string>{}(layerName); }
        std::size_t getTargetLayerHash() const { return mTargetLayerHash; }

        void setTexture(const std::string& texturePath) { mTexture = Texture(texturePath); }
        void setShaderProgram(ShaderProgram&& shaderProgram) { mShaderProgram = std::move(shaderProgram); }

    private:
        glm::mat4 mMatrix;
        Texture mTexture;
        ShaderProgram mShaderProgram;
        std::size_t mTargetLayerHash;
    };

} // namespace lake
