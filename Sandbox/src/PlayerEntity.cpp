#include "Lake/Entity.h"

#include "Lake/SpriteComponent.h"
#include "Lake/BoxColliderComponent.h"
#include "Lake/JsonHelpers.h"
#include "Lake/Event.h"
#include "Lake/Scene.h"
#include "Lake/CameraComponent.h"
#include "Lake/SoundComponent.h"

#include <glm/gtc/matrix_transform.hpp>

class PlayerEntity final : public lake::Entity {
    LK_REGISTER_ENTITY(PlayerEntity);

public:
    PlayerEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale({0.25f, 0.25f})
        , mMoveDirection(0)
        , mScore(0)
    { }

    ~PlayerEntity() override = default;

    void onCreate() override {
        this->addComponent<lake::DrawableComponent>(new lake::SpriteComponent("fruit-catcher-idle.png", "main"));

        this->addComponent<lake::SoundComponent>(new lake::SoundComponent("collect.mp3"));

        lake::BoxColliderComponent* collider = new lake::BoxColliderComponent(this, mScale, {mPosition.x, mPosition.y});
        collider->setCollisionCallback([this](lake::ColliderComponent* selfCollider, lake::ColliderComponent* otherCollider) {
            this->onCollision(otherCollider->getParent());
        });
        this->addComponent<lake::BoxColliderComponent>(collider);
    }

    void onUpdate(f32 timeStep) override {
        constexpr f32 cSpeed = 5.0f;
        mPosition.x += mMoveDirection * cSpeed * timeStep;

        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (lake::SpriteComponent* sprite = dynamic_cast<lake::SpriteComponent*>(component)) {
                sprite->setMatrix(
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(mScale, 1.0f))
                );
            }
        }

        std::span<lake::BoxColliderComponent*> boxColliders = this->getComponents<lake::BoxColliderComponent>();
        for (auto component : boxColliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y - 0.1f));
        }
    }

    void onEvent(lake::Event* event) override {
        if (event->getType() == lake::EventType::KeyPress) {
            lake::KeyPressEvent* e = dynamic_cast<lake::KeyPressEvent*>(event);
            if (e->getKey() == GLFW_KEY_LEFT || e->getKey() == GLFW_KEY_A) {
                mMoveDirection = -1;
            } else if (e->getKey() == GLFW_KEY_RIGHT || e->getKey() == GLFW_KEY_D) {
                mMoveDirection = 1;
            }
        } else if (event->getType() == lake::EventType::KeyRelease) {
            lake::KeyReleaseEvent* e = dynamic_cast<lake::KeyReleaseEvent*>(event);
            if (e->getKey() == GLFW_KEY_LEFT || e->getKey() == GLFW_KEY_RIGHT || e->getKey() == GLFW_KEY_A || e->getKey() == GLFW_KEY_D) {
                mMoveDirection = 0;
            }
        } else if (event->getType() == lake::EventType::MouseMove) {
            lake::MouseMoveEvent* e = dynamic_cast<lake::MouseMoveEvent*>(event);

            // Get the camera entity and unproject the mouse position to world coordinates.
            Entity* cameraEntity = *std::find_if(mScene->getEntities().begin(), mScene->getEntities().end(), [](Entity* entity) {
                return entity->getIdentifierHash() == std::hash<std::string>{}("MainCamera");
            });

            const lake::CameraComponent* camera = cameraEntity->getComponents<lake::CameraComponent>()[0];
            const glm::vec3 worldPos = camera->unProject(e->getPosition());

            mPosition.x = worldPos.x;
        }
    }

    void onCollision(lake::Entity* other) {
        if (other->hasTag("fruit")) {
            other->setAlive(false);

            lake::info("Fruit caught! Score: ", ++mScore);

            lake::SoundComponent* soundComponent = this->getComponents<lake::SoundComponent>()[0];
            soundComponent->setPitch(1.0f + (mScore / 1000.0f));
            soundComponent->play();
        }
    }

    glm::vec3 mPosition;
    glm::vec2 mScale;
    i8 mMoveDirection;
    u32 mScore;
};
