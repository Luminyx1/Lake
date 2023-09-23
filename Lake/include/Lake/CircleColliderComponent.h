#pragma once

#include "Lake/ColliderComponent.h"

namespace lake {

    class CircleColliderComponent final : public ColliderComponent {
    public:
        CircleColliderComponent(const f32 radius, const glm::vec2& position)
            : ColliderComponent(position)
            , mRadius(radius)
        { }

        ~CircleColliderComponent() override = default;

        [[nodiscard]] f32 getRadius() const { return mRadius; }
        void setRadius(const f32 radius) { mRadius = radius; }

    private:
        f32 mRadius;
    };

} // namespace lake
