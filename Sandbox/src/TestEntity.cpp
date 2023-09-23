#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/CircleColliderComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

class TestEntity final : public lake::Entity {
public:
    TestEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale(lake::json::getVec2(properties, "scale"))
        , mRotation(0.0f)
    {
        lake::SpriteComponent* sprite = new lake::SpriteComponent("sprite.png");
        sprite->setTargetLayer("main");
        this->addComponent<lake::DrawableComponent>(sprite);

        lake::CircleColliderComponent* collider = new lake::CircleColliderComponent((mScale.x + mScale.y) / 2.0f, glm::vec2(mPosition.x, mPosition.y));
        collider->setCollisionCallback([this](lake::ColliderComponent* self, lake::ColliderComponent* other) {
            mRotateNextFrame = true;
        });
        this->addComponent<lake::CircleColliderComponent>(collider);
    }

    ~TestEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        if (ImGui::Begin(("TestEntity" + std::to_string((int)(uintptr_t)this)).c_str())) {
            ImGui::SliderFloat3("Position", &mPosition.x, -10.0f, 10.0f);
        } ImGui::End();

        if (mRotateNextFrame) {
            mRotation += timeStep * 90.0f;
            mRotateNextFrame = false;
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

        std::span<lake::CircleColliderComponent*> circleColliders = this->getComponents<lake::CircleColliderComponent>();
        for (auto component : circleColliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

private:
    glm::vec3 mPosition;
    glm::vec2 mScale;
    f32 mRotation;
    bool mRotateNextFrame;
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
