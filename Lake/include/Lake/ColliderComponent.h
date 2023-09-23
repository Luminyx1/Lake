#pragma once

#include "Lake/Entity.h"

#include <functional>

namespace lake {

    class ColliderComponent : public EntityComponent {
    public:
        ColliderComponent(const glm::vec2& position)
            : mPosition(position)
            , mCollisionCallback(nullptr)
        { }

        ~ColliderComponent() override = default;

        [[nodiscard]] const glm::vec2& getPosition() const { return mPosition; }
        void setPosition(const glm::vec2& position) { mPosition = position; }

        void setCollisionCallback(const std::function<void(ColliderComponent*, ColliderComponent*)>& callback) { mCollisionCallback = callback; }

        void operator()(ColliderComponent* other) {
            if (mCollisionCallback) {
                mCollisionCallback(this, other);
            }
        }

    private:
        glm::vec2 mPosition;
        std::function<void(ColliderComponent*, ColliderComponent*)> mCollisionCallback;
    };

} // namespace lake
