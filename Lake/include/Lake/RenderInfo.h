#pragma once

#include "Lake/Common.h"

#include "Lake/CameraComponent.h"
#include "Lake/Framebuffer.h"

namespace lake {

    struct RenderInfo {
        CameraComponent* camera;
        const Framebuffer* framebuffer;
    };

} // namespace lake
