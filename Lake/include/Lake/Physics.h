#pragma once

#include "Lake/Common.h"
#include "Lake/CircleColliderComponent.h"

#include <span>

namespace lake {

    class Physics {
    public:
        Physics() = default;
        ~Physics() = default;

        void update(const f32 timeStep, std::span<CircleColliderComponent*> circleColliders);
    };

} // namespace lake
