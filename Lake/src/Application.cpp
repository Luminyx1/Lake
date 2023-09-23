#include "Lake/Application.h"

#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

lake::Application::Application(const lake::Application::Properties& properties)
    : mGraphics({
        .window = {
            .width = properties.window.width,
            .height = properties.window.height
        }
    })
    , mScene(properties.initialScene)
    , mPhysics()
{ }

lake::Application::~Application() {

}

void lake::Application::run() {
    this->intermoduleDataTransfer();

    while (mGraphics.update()) {
        const f32 ts = mGraphics.getTimeStep();

        this->onUpdate(ts);

        mScene.update(ts);

        this->intermoduleDataTransfer();
    }
}

void lake::Application::intermoduleDataTransfer() {
    { // Link graphics and scene
        const std::vector<Entity*>& entities = mScene.getEntities();

        for (auto entity : entities) {        
            for (auto drawableComponent : entity->getComponents<lake::DrawableComponent>()) {
                mGraphics.pushDrawable(drawableComponent, drawableComponent->getTargetLayerHash());
            }

            for (auto cameraComponent : entity->getComponents<lake::CameraComponent>()) {
                mGraphics.getLayerStack().getLayer(cameraComponent->getTargetLayerHash())->setCamera(cameraComponent);
            }
        }
    }
    { // Link physics and scene
        const std::vector<Entity*>& entities = mScene.getEntities();

        std::vector<lake::CircleColliderComponent*> circleColliders; circleColliders.reserve(entities.size());

        for (auto entity : entities) {
            for (auto circleCollider : entity->getComponents<lake::CircleColliderComponent>()) {
                circleColliders.push_back(circleCollider);
            }
        }

        mPhysics.update(mGraphics.getTimeStep(), circleColliders);
    }
}
