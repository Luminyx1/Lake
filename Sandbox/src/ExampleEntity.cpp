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
        this->addComponent<lake::SpriteComponent>(sprite);
    }

    ~TestEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        std::span<lake::SpriteComponent*> spriteComponents = this->getComponents<lake::SpriteComponent>();
        for (auto& component : spriteComponents) {
            component->setMatrix(
                glm::translate(glm::mat4(1.0f), mPosition)
            );
        }
    }
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
