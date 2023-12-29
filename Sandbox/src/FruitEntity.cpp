#include "Lake/Entity.h"

#include "Lake/SpriteComponent.h"
#include "Lake/BoxColliderComponent.h"
#include "Lake/JsonHelpers.h"
#include "Lake/Event.h"
#include "Lake/Scene.h"
#include "Lake/CameraComponent.h"
#include "Lake/TagComponent.h"
#include "Lake/SoundComponent.h"

#include <glm/gtc/matrix_transform.hpp>

class FruitEntity final : public lake::Entity {
    LK_REGISTER_ENTITY(FruitEntity);

public:
    FruitEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale({0.15f, 0.15f})
    { }

    ~FruitEntity() override = default;

    void onCreate() override {
        this->addComponent<lake::DrawableComponent>(new lake::SpriteComponent("apple.png", "main"));
        this->addComponent<lake::BoxColliderComponent>(new lake::BoxColliderComponent(this, mScale, {mPosition.x, mPosition.y}));
        this->addComponent<lake::TagComponent>(new lake::TagComponent("fruit"));
        this->addComponent<lake::SoundComponent>(new lake::SoundComponent("miss.mp3"));
    }

    void onUpdate(f32 timeStep) override {
        if (mPosition.y < -1.0f) {
            this->setAlive(false);
            this->getComponents<lake::SoundComponent>()[0]->play();
            return;
        }

        constexpr f32 cSpeed = 5.0f;
        mPosition.y -= cSpeed * timeStep;

        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (lake::SpriteComponent* sprite = dynamic_cast<lake::SpriteComponent*>(component)) {
                sprite->setMatrix(
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(mScale, 0.0f))
                );
            }
        }

        std::span<lake::BoxColliderComponent*> boxColliders = this->getComponents<lake::BoxColliderComponent>();
        for (auto component : boxColliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

    glm::vec3 mPosition;
    glm::vec2 mScale;
};
