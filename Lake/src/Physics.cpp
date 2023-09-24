#include "Lake/Physics.h"

#include "Lake/Log.h"

void lake::Physics::update(const f32 timeStep, std::span<CircleColliderComponent*> circleColliders, std::span<BoxColliderComponent*> boxColliders) {
    // circle-circle collision
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

    // circle-box collision
    for (auto circleCollider : circleColliders) {
        for (auto boxCollider : boxColliders) {
            const glm::vec2& circleColliderPosition = circleCollider->getPosition();
            const glm::vec2& boxColliderPosition = boxCollider->getPosition();

            glm::vec2 closestPoint;
            closestPoint.x = glm::clamp(circleCollider->getPosition().x, boxCollider->getPosition().x - boxCollider->getSize().x, boxCollider->getPosition().x + boxCollider->getSize().x);
            closestPoint.y = glm::clamp(circleCollider->getPosition().y, boxCollider->getPosition().y - boxCollider->getSize().y, boxCollider->getPosition().y + boxCollider->getSize().y);

            const glm::vec2 circleToClosest = closestPoint - circleCollider->getPosition();

            if (glm::length(circleToClosest) < circleCollider->getRadius()) {
                circleCollider->operator()(boxCollider);
                boxCollider->operator()(circleCollider);
            }
        }
    }

    // box-box collision
    for (auto boxCollider : boxColliders) {
        for (auto otherBoxCollider : boxColliders) {
            if (boxCollider == otherBoxCollider) continue;

            const glm::vec2& boxColliderPosition = boxCollider->getPosition();
            const glm::vec2& otherBoxColliderPosition = otherBoxCollider->getPosition();

            const glm::vec2 boxColliderSize = boxCollider->getSize();
            const glm::vec2 otherBoxColliderSize = otherBoxCollider->getSize();

            const glm::vec2 boxColliderMin = boxColliderPosition - boxColliderSize;
            const glm::vec2 boxColliderMax = boxColliderPosition + boxColliderSize;

            const glm::vec2 otherBoxColliderMin = otherBoxColliderPosition - otherBoxColliderSize;
            const glm::vec2 otherBoxColliderMax = otherBoxColliderPosition + otherBoxColliderSize;

            if (boxColliderMin.x < otherBoxColliderMax.x && boxColliderMax.x > otherBoxColliderMin.x &&
                boxColliderMin.y < otherBoxColliderMax.y && boxColliderMax.y > otherBoxColliderMin.y
            ) {
                boxCollider->operator()(otherBoxCollider);
            }
        }
    }
}
