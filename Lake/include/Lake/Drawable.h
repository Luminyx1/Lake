#pragma once

#include "Lake/Common.h"

namespace lake {

    class LK_API Drawable {
    public:
        Drawable() = default;
        virtual ~Drawable() = default;

        virtual void draw() = 0;
    };

} // namespace lake
