#include "Lake/Scene.h"

#include "Lake/Log.h"

#include <simdjson.h>

lake::Scene::Scene(const std::string& path) {
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

            mEntities.push_back(newEntity);
        }
    } catch (simdjson::simdjson_error& error) {
        lake::error("Error parsing scene file '", path, "': ", error.what());
    }
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
    }
}
