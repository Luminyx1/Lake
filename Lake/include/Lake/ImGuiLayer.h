#pragma once

#include "Lake/Common.h"

#include "Lake/Layer.h"

#include <string>

namespace lake {

    class LK_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer(const std::string& name);
        ~ImGuiLayer();

        void draw() override;
    };

} // namespace lake
