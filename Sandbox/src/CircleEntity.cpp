#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/CircleColliderComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"
#include "Lake/Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <algorithm>

class CircleEntity final : public lake::Entity {
    //* Register our class so we can create it from a scene file.
    LK_REGISTER_ENTITY(CircleEntity);

public:
    CircleEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale(lake::json::getVec2(properties, "scale"))
        , mRotation(0.0f)
        , mMousePos(0.0f, 0.0f)
        , mRotateNextFrame(false)
        , mMousedown(false)
        , mTime(0.0f)
    {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add a sprite component to draw a sprite.
        lake::SpriteComponent* sprite = new lake::SpriteComponent("circle.png");
        sprite->setTargetLayer("main"); // Assign the sprite to be drawn in the "main" layer.
        this->addComponent<lake::DrawableComponent>(sprite);

        // Add a collider component to detect collisions. Shape is circle, and we can register a callback for when a collision occurs.
        lake::CircleColliderComponent* collider = new lake::CircleColliderComponent(this, (mScale.x + mScale.y) / 2.0f, glm::vec2(mPosition.x, mPosition.y));
        collider->setCollisionCallback([this](lake::ColliderComponent* self, lake::ColliderComponent* other) {
            static const std::size_t targetHash = std::hash<std::string>{}("BoxEntity"); // This is the entity we are interested in checking collisions with.

            if (other->getParent()->getIdentifierHash() == targetHash) {
                mRotateNextFrame = true;
            }
        });
        this->addComponent<lake::CircleColliderComponent>(collider);
    }

    ~CircleEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        //* Act behaviour for our entity. This is called every frame.

        // Rotate if we are intersecting the mouse or colliding with another entity.
        if (mRotateNextFrame || mMousedown != 0) {
            mRotation += timeStep * 1800.0f * (mMousedown != 0) ? mMousedown : 1;
            mRotateNextFrame = false;
        }

        // Get the camera entity and unproject the mouse position to world coordinates.
        Entity* cameraEntity = *std::find_if(mScene->getEntities().begin(), mScene->getEntities().end(), [](Entity* entity) {
            return entity->getIdentifierHash() == std::hash<std::string>{}("MainCamera");
        });

        const lake::CameraComponent* camera = cameraEntity->getComponents<lake::CameraComponent>()[0];
        const glm::vec3 worldPos = camera->unProject(mMousePos);

        // Check if we are intersecting the mouse, and if so, rotate next frame.
        if (this->getComponents<lake::CircleColliderComponent>()[0]->intersects(worldPos)) {
            mRotateNextFrame = true;
        }

        // Smoothly move left and right with consideration for the time step. Sin is used to make the movement smooth.
        mTime += timeStep;
        mPosition.x = glm::sin(glm::radians(mTime * 100.0f));

        // Update the sprite component's matrix to reflect the new position, rotation and scale. We use a for loop here because we can have multiple drawable components, however in this case we only have one.
        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (lake::SpriteComponent* sprite = dynamic_cast<lake::SpriteComponent*>(component)) {
                sprite->setMatrix(
                    // Transformation must be in this order: translate, rotate, scale.
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(mScale, 0.0f))
                );
            }
        }

        // Update the collider position. We use a for loop here because we can have multiple collider components, however in this case we only have one.
        std::span<lake::CircleColliderComponent*> colliders = this->getComponents<lake::CircleColliderComponent>();
        for (auto component : colliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

    void onEvent(lake::Event* event) override {
        //* Track events to get the mouse position and mouse button state. We only act in onUpdate, so just store information here.

        if (event->getType() == lake::EventType::MouseMove) {
            const auto mouseMoveEvent = static_cast<lake::MouseMoveEvent*>(event);

            mMousePos = mouseMoveEvent->getPosition();
        } else if (event->getType() == lake::EventType::MousePress) {
            const auto mousePressEvent = static_cast<lake::MousePressEvent*>(event);

            if (mousePressEvent->getButton() == GLFW_MOUSE_BUTTON_LEFT) {
                mMousedown = 1;
            } else if (mousePressEvent->getButton() == GLFW_MOUSE_BUTTON_RIGHT) {
                mMousedown = -1;
            }
        } else if (event->getType() == lake::EventType::MouseRelease) {
            const auto mouseReleaseEvent = static_cast<lake::MouseReleaseEvent*>(event);

            if (mouseReleaseEvent->getButton() == GLFW_MOUSE_BUTTON_LEFT || mouseReleaseEvent->getButton() == GLFW_MOUSE_BUTTON_RIGHT) {
                mMousedown = 0;
            }
        }
    }

private:
    glm::vec3 mPosition;
    glm::vec2 mScale;
    f32 mRotation;
    glm::vec2 mMousePos;
    bool mRotateNextFrame;
    i32 mMousedown;
    f32 mTime;
};
