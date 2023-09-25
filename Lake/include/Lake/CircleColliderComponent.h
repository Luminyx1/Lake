#pragma once

#include "Lake/ColliderComponent.h"

namespace lake {

    class CircleColliderComponent final : public ColliderComponent {
    public:
        CircleColliderComponent(Entity* parent, const f32 radius, const glm::vec2& position)
            : ColliderComponent(parent, position)
            , mRadius(radius)
        { }

        ~CircleColliderComponent() override = default;

        [[nodiscard]] f32 getRadius() const { return mRadius; }
        void setRadius(const f32 radius) { mRadius = radius; }

        bool intersects(const glm::vec2& point) const {
            const glm::vec2 delta = point - mPosition;
            return glm::dot(delta, delta) <= mRadius * mRadius;
        }

    private:
        f32 mRadius;
    };

} // namespace lake
