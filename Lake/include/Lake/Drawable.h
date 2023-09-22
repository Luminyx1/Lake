#pragma once

#include "Lake/Common.h"

namespace lake {

    class Drawable {
    public:
        Drawable() = default;
        virtual ~Drawable() = default;

        virtual void draw() = 0;
    };

} // namespace lake
