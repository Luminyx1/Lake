#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/Graphics.h"

#include <glm/gtc/matrix_transform.hpp>

class TestEntity final : public lake::Entity {
public:
    TestEntity(const lake::Entity::Properties& properties)
        : Entity(properties)
    {
        lake::SpriteComponent* sprite = new lake::SpriteComponent("sprite.png");
        sprite->setTargetLayer("main");
        this->addComponent<lake::DrawableComponent>(sprite);
    }

    ~TestEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto& component : drawableComponents) {
            if (dynamic_cast<lake::SpriteComponent*>(component)) {
                dynamic_cast<lake::SpriteComponent*>(component)->setMatrix(
                    glm::translate(glm::mat4(1.0f), mPosition)
                );
            }
        }
    }
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
