#pragma once

#include "Lake/ColliderComponent.h"

namespace lake {

    class BoxColliderComponent final : public ColliderComponent {
    public:
        BoxColliderComponent(const glm::vec2& size, const glm::vec2& position)
            : ColliderComponent(position)
            , mSize(size)
        { }

        ~BoxColliderComponent() override = default;

        [[nodiscard]] glm::vec2 getSize() const { return mSize; }
        void setSize(const glm::vec2& size) { mSize = size; }

    private:
        glm::vec2 mSize;
    };

} // namespace lake
