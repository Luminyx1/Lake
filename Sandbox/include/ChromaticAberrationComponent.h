#pragma once

#include "Lake/DrawableComponent.h"

#include "Lake/Framebuffer.h"
#include "Lake/ShaderProgram.h"

class ChromaticAberrationComponent : public lake::DrawableComponent {
public:
    ChromaticAberrationComponent(const std::string& layerName);

    void draw(const lake::RenderInfo& renderInfo) override;
    void resize(const glm::u32vec2& size);

private:
    lake::Framebuffer mWorkBuffer;
    lake::ShaderProgram mShaderProgram;
};
