#include "Lake/Scene.h"

#include "Lake/Log.h"
#include "Lake/TagComponent.h"
#include "Lake/Application.h"

#include <simdjson.h>

lake::Scene::Scene(const std::string& path) {
    this->loadScene(path);
}

lake::Scene::~Scene() {
    for (auto& entity : mEntities) {
        delete entity;
    }
}

void lake::Scene::update(const f32 timeStep) {
    for (auto entity : mEntities) {
        entity->onUpdate(timeStep);
    }
}

void lake::Scene::onEvent(lake::Event* event) {
    for (auto entity : mEntities) {
        entity->onEvent(event);

        for (auto& [hash, componentVector] : entity->mComponents) {
            for (lake::EntityComponent* component : componentVector) {
                component->onEvent(event);
            }
        }
    }
}

void lake::Scene::switchScene(const std::string& path) {
    Application::raiseEvent(new SceneSwitchEvent(path));

    for (auto& entity : mEntities) {
        delete entity;
    }

    mEntities.clear();

    this->loadScene(path);
}

void lake::Scene::loadScene(const std::string& path) {
    /**
     * TODO: Preload assets while running the current scene before switching to the new scene to avoid stuttering (including audio)
     * * NOTE: Move SceneSwitchEvent firing to after the assets have been loaded because the audio cache might be cleared too early
    */

    mPath = path;

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document document = parser.iterate(json);

    try {
        auto entities = document["entities"].get_array();

        for (auto entity : entities) {
            auto entityObject = entity.value().get_object();

            const std::string_view type = entityObject["type"].get_string().value();
            const lake::Entity::Properties properties = entityObject["properties"].get_object();

            const auto& registry = Entity::Registry::getRegistry();
            const auto entityRegistration = registry.find(std::string{type});
            if (entityRegistration == registry.end()) {
                lake::error("Entity '", type, "' not found in registry");
                continue;
            }

            const auto& [name, data] = *entityRegistration;

            Entity* newEntity = data.factory(properties);

            newEntity->mRegistry = &data;
            newEntity->mScene = this;
            auto tags = entityObject["properties"]["tags"];
            if (tags.error() != simdjson::error_code::NO_SUCH_FIELD) { //? Should we set these here or in the constructor?
                for (auto tag : tags.get_array()) {
                    newEntity->addComponent<lake::TagComponent>(new lake::TagComponent(std::string{tag.get_string().value()}));
                }
            }

            mEntities.push_back(newEntity);
        }
    } catch (simdjson::simdjson_error& error) {
        lake::error("Error parsing scene file '", path, "': ", error.what());
    }
}
