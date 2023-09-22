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
            auto properties = entityObject["properties"].get_object();

            auto position = properties["position"].get_array();

            auto posIt = position.begin();

            const f32 x = static_cast<f32>(f64(*posIt.value()));
            const f32 y = static_cast<f32>(f64(*(++posIt).value()));
            const f32 z = static_cast<f32>(f64(*(++posIt).value()));

            const Entity::Properties entityProperties = {
                .position = glm::vec3(x, y, z)
            };

            const auto& registry = Entity::Registry::getRegistry();
            const auto entityRegistration = registry.find(std::string{type});
            if (entityRegistration == registry.end()) {
                lake::error("Entity '", type, "' not found in registry");
                continue;
            }

            const auto& [name, data] = *entityRegistration;

            mEntities.push_back(data.factory(entityProperties));
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

void lake::Scene::update(const f32 ts) {
    for (auto& entity : mEntities) {
        entity->onUpdate(ts);
    }
}
