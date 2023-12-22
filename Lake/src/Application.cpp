#include "Lake/Application.h"

#include "Lake/Log.h"
#include "Lake/SpriteComponent.h"
#include "Lake/SoundComponent.h"

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
    , mAudio()
{ }

lake::Application::~Application() {

}

void lake::Application::run() {
    this->intermoduleDataTransfer();

    do { // Main loop
        const f32 ts = mGraphics.getTimeStep();

        this->onUpdate(ts);
        this->handleEvents();

        mScene.update(ts);

        this->intermoduleDataTransfer();
    } while (mGraphics.update());
}

void lake::Application::handleEvents() {
    std::deque<lake::Event*> eventQueueCopy;
    eventQueueCopy.swap(sEventQueue);

    for (auto& event : eventQueueCopy) {
        this->onEventInternal(event);
        this->onEvent(event);

        mScene.onEvent(event);
        mGraphics.onEvent(event);

        delete event;
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
    { // Link audio and scene
        const std::vector<Entity*>& entities = mScene.getEntities();
        
        std::vector<lake::SoundComponent*> soundComponents; soundComponents.reserve(entities.size());

        for (auto entity : entities) {
            for (auto soundComponent : entity->getComponents<lake::SoundComponent>()) {
                soundComponents.push_back(soundComponent);
            }
        }

        mAudio.update(soundComponents);
    }
}

void lake::Application::onEventInternal(lake::Event* event) {
    if (event->getType() == lake::EventType::SceneSwitch) {
        const auto sceneSwitchEvent = static_cast<lake::SceneSwitchEvent*>(event);

        mAudio.clearCache();
    }
}

void lake::Application::raiseEvent(lake::Event* event) {
    sEventQueue.push_back(event);
}
