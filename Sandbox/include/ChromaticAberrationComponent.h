#pragma once

#include "Lake/DrawableComponent.h"

#include "Lake/Framebuffer.h"
#include "Lake/ShaderProgram.h"

class ChromaticAberrationComponent : public lake::DrawableComponent {
public:
    ChromaticAberrationComponent(const std::string& layerName);

    void onEvent(lake::Event* event) override;
    void draw(const lake::RenderInfo& renderInfo) override;

private:
    lake::Framebuffer mWorkBuffer;
    lake::ShaderProgram mShaderProgram;
};
