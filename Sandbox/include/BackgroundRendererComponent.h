#pragma once

#include "Lake/DrawableComponent.h"

#include "Lake/Framebuffer.h"
#include "Lake/ShaderProgram.h"

class BackgroundRendererComponent final : public lake::DrawableComponent {
public:
    BackgroundRendererComponent(const std::string& layerName);
    void onEvent(lake::Event* event) override;
    void draw(const lake::RenderInfo& renderInfo) override;

    void update(f32 timeStep);

private:
    lake::Framebuffer mWorkBuffer;
    lake::ShaderProgram mShaderProgram;
    f32 mTime;
};
