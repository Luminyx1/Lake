#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/BoxColliderComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

class TestEntity1 final : public lake::Entity {
public:
    TestEntity1(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale(lake::json::getVec2(properties, "scale"))
        , mRotation(0.0f)
        , mDirectionX(0)
        , mDirectionY(0)
        , mRotateNextFrame(false)
    {
        lake::SpriteComponent* sprite = new lake::SpriteComponent("box.png");
        sprite->setTargetLayer("main");
        this->addComponent<lake::DrawableComponent>(sprite);

        lake::BoxColliderComponent* collider = new lake::BoxColliderComponent(mScale, glm::vec2(mPosition.x, mPosition.y));
        collider->setCollisionCallback([this](lake::ColliderComponent* self, lake::ColliderComponent* other) {
            //mRotateNextFrame = true;
        });
        this->addComponent<lake::BoxColliderComponent>(collider);
    }

    ~TestEntity1() override = default;

    void onUpdate(const f32 timeStep) override {
        if (mRotateNextFrame) {
            mRotation += timeStep * -180.0f;
            mRotateNextFrame = false;
        }

        mPosition.x += timeStep * mDirectionX * 10.0f;
        mPosition.y += timeStep * mDirectionY * 10.0f;

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

        std::span<lake::BoxColliderComponent*> boxColliders = this->getComponents<lake::BoxColliderComponent>();
        for (auto component : boxColliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

    void onEvent(lake::Event* event) override {
        if (event->getType() == lake::EventType::KeyPress) {
            const auto keyEvent = static_cast<lake::KeyPressEvent*>(event);

            if (keyEvent->getKey() == GLFW_KEY_W) {
                mDirectionY = 1;
            } else if (keyEvent->getKey() == GLFW_KEY_S) {
                mDirectionY = -1;
            } else if (keyEvent->getKey() == GLFW_KEY_A) {
                mDirectionX = -1;
            } else if (keyEvent->getKey() == GLFW_KEY_D) {
                mDirectionX = 1;
            }
        } else if (event->getType() == lake::EventType::KeyRelease) {
            const auto keyEvent = static_cast<lake::KeyReleaseEvent*>(event);

            if (keyEvent->getKey() == GLFW_KEY_W) {
                mDirectionY = 0;
            } else if (keyEvent->getKey() == GLFW_KEY_S) {
                mDirectionY = 0;
            } else if (keyEvent->getKey() == GLFW_KEY_A) {
                mDirectionX = 0;
            } else if (keyEvent->getKey() == GLFW_KEY_D) {
                mDirectionX = 0;
            }
        }
    }

private:
    glm::vec3 mPosition;
    glm::vec2 mScale;
    f32 mRotation;
    i32 mDirectionX, mDirectionY;
    bool mRotateNextFrame;
};

lake::Entity::RegisterEntity<TestEntity1> testEntity1("TestEntity1");
