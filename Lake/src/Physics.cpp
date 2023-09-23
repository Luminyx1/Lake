#include "Lake/Physics.h"

#include "Lake/Log.h"

void lake::Physics::update(const f32 timeStep, std::span<CircleColliderComponent*> circleColliders) {
    for (auto circleCollider : circleColliders) {
        for (auto otherCircleCollider : circleColliders) {
            if (circleCollider == otherCircleCollider) continue;

            const glm::vec2& circleColliderPosition = circleCollider->getPosition();
            const glm::vec2& otherCircleColliderPosition = otherCircleCollider->getPosition();

            const f32 distance = glm::distance(circleColliderPosition, otherCircleColliderPosition);

            if (distance < circleCollider->getRadius() + otherCircleCollider->getRadius()) {
                circleCollider->operator()(otherCircleCollider);
            }
        }
    }
}
