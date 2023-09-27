#pragma once

#include "Lake/ColliderComponent.h"

namespace lake {

    class BoxColliderComponent final : public ColliderComponent {
    public:
        BoxColliderComponent(Entity* parent, const glm::vec2& size, const glm::vec2& position)
            : ColliderComponent(parent, position)
            , mSize(size)
        { }

        ~BoxColliderComponent() override = default;

        [[nodiscard]] glm::vec2 getSize() const { return mSize; }
        void setSize(const glm::vec2& size) { mSize = size; }

        bool intersects(const glm::vec2& point) const {
            const glm::vec2& pos = this->getPosition();
            const glm::vec2& size = this->getSize();

            return point.x >= pos.x - size.x && point.x <= pos.x + size.x &&
                   point.y >= pos.y - size.y && point.y <= pos.y + size.y;
        }

    private:
        glm::vec2 mSize;
    };

} // namespace lake
