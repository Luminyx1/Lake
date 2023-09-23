#pragma once

#include "Lake/Common.h"

#include "Lake/Layer.h"

#include <string>

namespace lake {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(const std::string& name);
        ~ImGuiLayer();

        void draw(const RenderInfo& renderInfo) override;
    };

} // namespace lake
