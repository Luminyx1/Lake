#include "Lake/Layer.h"

class CustomRenderLayer : public lake::Layer {
public:
    CustomRenderLayer(const std::string& name)
        : Layer(name)
    { }

    void draw(const lake::RenderInfo& renderInfo) override;
};
