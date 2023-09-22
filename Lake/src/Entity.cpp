#include "Lake/Entity.h"

lake::Entity::Registry::Registry(const lake::Entity::Registry::entityFactory factory, const std::string& identifier) 
    : factory(factory)
    , identifier(identifier)
{
    Registry::getRegistry()[identifier] = *this;
}

std::map<std::string, lake::Entity::Registry>& lake::Entity::Registry::getRegistry() {
    static std::map<std::string, lake::Entity::Registry> registry;

    return registry;
}

lake::Entity::Entity(const lake::Entity::Properties& properties) 
    : mPosition(properties.position)
{ }

lake::Entity::~Entity() {
    for (auto& [type, vector] : mComponents) {
        for (auto& component : vector) {
            delete component;
        }
    }

    mComponents.clear();
}
