#pragma once

#include "Lake/Common.h"

#include "Lake/Layer.h"

namespace lake {

    class Graphics {
    public:
        struct Properties {
            struct WindowProperties {
                u32 width, height;
            } window;
        };

    public:
        Graphics(const Properties& properties);
        ~Graphics();

        [[nodiscard]] bool update();

        void pushDrawable(Drawable* drawable, const std::size_t layerHash);

        [[nodiscard]] f32 getTimeStep() const { return mTimeStep; }
        [[nodiscard]] LayerStack& getLayerStack() { return mLayerStack; }

    private:
        f32 mTimeStep, mFrameTime, mLastFrameTime;
        LayerStack mLayerStack;
    };

} // namespace lake
