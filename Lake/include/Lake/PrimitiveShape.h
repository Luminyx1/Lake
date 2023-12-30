#pragma once

#include "Lake/Common.h"

namespace lake {

    class PrimitiveShape {
    public:
        static void init();

        // TODO: Return class wrappers

        // Quad
        static [[nodiscard]] const u32 getQuadVBO() { return sQuadVBO; }
        static [[nodiscard]] const u32 getQuadEBO() { return sQuadEBO; }
        static [[nodiscard]] const u32 getQuadVAO() { return sQuadVAO; }

    private:
        static u32 sQuadVBO, sQuadEBO, sQuadVAO;
    };

} // namespace lake
