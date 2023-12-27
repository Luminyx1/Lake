#pragma once

#include "Lake/DrawableComponent.h"

#include "Lake/Framebuffer.h"
#include "Lake/ShaderProgram.h"

namespace lake {

    class FXAAComponent : public lake::DrawableComponent {
    public:
        FXAAComponent(const std::string& layerName);

        void onEvent(lake::Event* event) override;
        void draw(const lake::RenderInfo& renderInfo) override;

    private:
        lake::Framebuffer mWorkBuffer;
        lake::ShaderProgram mShaderProgram;
    };

}
