#include "Lake/Entity.h"
#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/CameraComponent.h"
#include "Lake/BoxColliderComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"
#include "Lake/TagComponent.h"
#include "Lake/Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

class BoxEntity final : public lake::Entity {
    //* Register our class so we can create it from a scene file.
    LK_REGISTER_ENTITY(BoxEntity);

public:
    BoxEntity(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mScale(lake::json::getVec2(properties, "scale"))
        , mDirectionX(0)
        , mDirectionY(0)
    {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add a sprite component to draw a sprite.
        lake::SpriteComponent* sprite = new lake::SpriteComponent("box.png");
        sprite->setTargetLayer("main"); // Assign the sprite to be drawn in the "main" layer.
        this->addComponent<lake::DrawableComponent>(sprite);

        // Add a collider component so other entities can collide with us. Shape is Box (rectangular), and don't register a callback since we don't need to act here.
        this->addComponent<lake::BoxColliderComponent>(new lake::BoxColliderComponent(this, mScale, glm::vec2(mPosition.x, mPosition.y)));

        // Add a tag component so we can identify this entity later. This can also be done with properties in the scene file to add it to a specific instance.
        //this->addComponent<lake::TagComponent>(new lake::TagComponent("specificBoxNumber1"));
    }

    ~BoxEntity() override = default;

    void onUpdate(const f32 timeStep) override {
        //* Act behaviour for our entity. This is called every frame.

        // Move the entity based on the direction we want to move in. Move values are determined by onEvent (keyboard input).
        mPosition += glm::vec3(timeStep * mDirectionX, timeStep * mDirectionY, 0.0f);

        // Update the sprite component's matrix to reflect the new position and scale. We use a for loop here because we can have multiple drawable components, however in this case we only have one.
        std::span<lake::DrawableComponent*> drawableComponents = this->getComponents<lake::DrawableComponent>();
        for (auto component : drawableComponents) {
            if (lake::SpriteComponent* sprite = dynamic_cast<lake::SpriteComponent*>(component)) {
                sprite->setMatrix(
                    // Transformation must be in this order: translate, rotate, scale. We don't rotate here, so skip that.
                    glm::translate(glm::mat4(1.0f), mPosition) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(mScale, 0.0f))
                );
            }
        }

        // Update the collider component's position. We use a for loop here because we can have multiple collider components, however in this case we only have one.
        std::span<lake::BoxColliderComponent*> boxColliders = this->getComponents<lake::BoxColliderComponent>();
        for (auto component : boxColliders) {
            component->setPosition(glm::vec2(mPosition.x, mPosition.y));
        }
    }

    void onEvent(lake::Event* event) override {
        //* Track events to get the state of keys we are interested in. We only act in onUpdate, so just store information here.

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

            else if (keyEvent->getKey() == GLFW_KEY_R) {
                // Reload the scene. This is useful for testing changes to the scene file.
                mScene->switchScene(mScene->getPath());
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
    i32 mDirectionX, mDirectionY;
};
