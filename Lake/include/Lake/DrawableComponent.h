#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"
#include "Lake/RenderInfo.h"

namespace lake {

    class DrawableComponent : public EntityComponent {
    public:
        virtual void draw(const RenderInfo& RenderInfo) = 0;
    };

} // namespace lake
