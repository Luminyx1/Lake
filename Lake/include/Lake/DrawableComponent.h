#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"
#include "Lake/RenderInfo.h"

namespace lake {

    class DrawableComponent : public EntityComponent {
    public:
        virtual void draw(const RenderInfo& RenderInfo) = 0;

        void setTargetLayer(const std::string& layerName) { mTargetLayerHash = std::hash<std::string>{}(layerName); }
        std::size_t getTargetLayerHash() const { return mTargetLayerHash; }

    private:
        std::size_t mTargetLayerHash;
    };

} // namespace lake
