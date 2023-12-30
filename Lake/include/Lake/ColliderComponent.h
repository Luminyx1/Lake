#pragma once

#include "Lake/Entity.h"

#include <functional>

namespace lake {

    class ColliderComponent : public EntityComponent {
    public:
        ColliderComponent(Entity* parent, const glm::vec2& position)
            : mPosition(position)
            , mCollisionCallback(nullptr)
            , mParent(parent)
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

        [[nodiscard]] Entity* getParent() const { return mParent; }

    protected:
        Entity* mParent;
        glm::vec2 mPosition;
        std::function<void(ColliderComponent*, ColliderComponent*)> mCollisionCallback;
    };

} // namespace lake
