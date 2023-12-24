#pragma once

#include "Lake/Common.h"

#include "Lake/Layer.h"
#include "Lake/Event.h"

#include <glm/glm.hpp>

namespace lake {

    class Graphics final {
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
        void onEvent(Event* event);

        void pushDrawable(DrawableComponent* drawable, const std::size_t layerHash);

        [[nodiscard]] f32 getTimeStep() const { return mTimeStep; }
        [[nodiscard]] LayerStack& getLayerStack() { return *mLayerStack; }

        static glm::u32vec2 getFramebufferSize();

    private:
        f32 mTimeStep, mFrameTime, mLastFrameTime;
        LayerStack* mLayerStack;
    };


} // namespace lake
