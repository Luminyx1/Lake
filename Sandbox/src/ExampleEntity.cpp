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

        const f32 aspectRatio = static_cast<f32>(lake::Graphics::getFramebufferSize().x) / static_cast<f32>(lake::Graphics::getFramebufferSize().y);
        lake::OrthographicCameraComponent* camera = new lake::OrthographicCameraComponent(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f, -1.0f, aspectRatio, -aspectRatio, -1.0f, 1.0f
        );
        camera->setTargetLayer("main");
        this->addComponent<lake::CameraComponent>(camera);
    }

    ~TestEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        std::span<lake::SpriteComponent*> spriteComponents = this->getComponents<lake::SpriteComponent>();
        for (auto& component : spriteComponents) {
            component->setMatrix(
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f))
            );
        }
    }
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
