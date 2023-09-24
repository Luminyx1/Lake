#include "Lake/Application.h"

#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::deque<lake::Event*> lake::Application::sEventQueue;

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

    while (mGraphics.update()) { // Main loop
        const f32 ts = mGraphics.getTimeStep();

        this->onUpdate(ts);
        this->handleEvents();

        mScene.update(ts);

        this->intermoduleDataTransfer();
    }
}

void lake::Application::handleEvents() {
    for (auto& event : sEventQueue) {
        this->onEvent(event);

        mScene.onEvent(event);
        mGraphics.onEvent(event);

        delete event;
    }

    sEventQueue.clear();
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
        std::vector<lake::BoxColliderComponent*> boxColliders; boxColliders.reserve(entities.size());

        for (auto entity : entities) {
            for (auto circleCollider : entity->getComponents<lake::CircleColliderComponent>()) {
                circleColliders.push_back(circleCollider);
            }

            for (auto boxCollider : entity->getComponents<lake::BoxColliderComponent>()) {
                boxColliders.push_back(boxCollider);
            }
        }

        mPhysics.update(mGraphics.getTimeStep(), circleColliders, boxColliders);
    }
}

void lake::Application::raiseEvent(lake::Event* event) {
    sEventQueue.push_back(event);
}
