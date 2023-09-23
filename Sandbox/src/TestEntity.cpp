#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

class TestEntity final : public lake::Entity {
public:
    TestEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mRotation(0.0f)
    {
        lake::SpriteComponent* sprite = new lake::SpriteComponent("sprite.png");
        sprite->setTargetLayer("main");
        this->addComponent<lake::DrawableComponent>(sprite);
    }

    ~TestEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        if (ImGui::Begin("TestEntity")) {
            ImGui::SliderFloat3("Position", &mPosition.x, -10.0f, 10.0f);
        } ImGui::End();

        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (dynamic_cast<lake::SpriteComponent*>(component)) {
                dynamic_cast<lake::SpriteComponent*>(component)->setMatrix(
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f))
                );
            }
        }

        mRotation += 90.0f * timeStep;
    }

private:
    glm::vec3 mPosition;
    f32 mRotation;
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
