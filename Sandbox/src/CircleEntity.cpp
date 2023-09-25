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
public:
    CircleEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale(lake::json::getVec2(properties, "scale"))
        , mRotation(0.0f)
        , mMousePos(0.0f, 0.0f)
        , mRotateNextFrame(false)
        , mMousedown(false)
    {
        lake::SpriteComponent* sprite = new lake::SpriteComponent("circle.png");
        sprite->setTargetLayer("main");
        this->addComponent<lake::DrawableComponent>(sprite);

        lake::CircleColliderComponent* collider = new lake::CircleColliderComponent(this, (mScale.x + mScale.y) / 2.0f, glm::vec2(mPosition.x, mPosition.y));
        collider->setCollisionCallback([this](lake::ColliderComponent* self, lake::ColliderComponent* other) {
            mRotateNextFrame = true;

            static const std::size_t targetHash = std::hash<std::string>{}("BoxEntity");

            if (other->getParent()->getIdentifierHash() == targetHash) {
                lake::info("I (", self->getParent()->getIdentifier(), ") collided with a BoxEntity!");
            }
        });
        this->addComponent<lake::CircleColliderComponent>(collider);
    }

    ~CircleEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        if (mRotateNextFrame) {
            mRotation += timeStep * 180.0f;
            mRotateNextFrame = false;
        } else {
            mRotation += timeStep * 180.0f * mMousedown;
        }

        Entity* cameraEntity = *std::find_if(mScene->getEntities().begin(), mScene->getEntities().end(), [](Entity* entity) {
            return entity->getIdentifierHash() == std::hash<std::string>{}("MainCamera");
        });

        // use camera to project mouse position
        const auto camera = static_cast<lake::OrthographicCameraComponent*>(cameraEntity->getComponents<lake::CameraComponent>()[0]);
        const glm::mat4 view = camera->getView();
        const glm::mat4 projection = camera->getProjection();
        const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, lake::Graphics::getFramebufferSize().x, lake::Graphics::getFramebufferSize().y);
        const glm::vec3 screenPos = glm::unProject(glm::vec3(mMousePos, 0.0f), view, projection, viewport);
        mPosition.x = screenPos.x;

        if (this->getComponents<lake::CircleColliderComponent>()[0]->intersects(screenPos)) {
            lake::info("I (", this->getIdentifier(), ") am intersecting the mouse!");
        }

        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (dynamic_cast<lake::SpriteComponent*>(component)) {
                dynamic_cast<lake::SpriteComponent*>(component)->setMatrix(
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(mScale, 0.0f))
                );
            }
        }

        std::span<lake::CircleColliderComponent*> colliders = this->getComponents<lake::CircleColliderComponent>();
        for (auto component : colliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

    void onEvent(lake::Event* event) override {
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
};

lake::Entity::RegisterEntity<CircleEntity> circleEntity("CircleEntity");
