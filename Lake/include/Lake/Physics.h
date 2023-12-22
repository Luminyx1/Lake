#pragma once

#include "Lake/Common.h"
#include "Lake/CircleColliderComponent.h"
#include "Lake/BoxColliderComponent.h"

#include <span>

namespace lake {

    class Physics final {
    public:
        Physics() = default;
        ~Physics() = default;

        void update(const f32 timeStep, std::span<CircleColliderComponent*> circleColliders, std::span<BoxColliderComponent*> boxColliders);
    };

} // namespace lake
