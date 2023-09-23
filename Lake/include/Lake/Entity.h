#pragma once

#include "Lake/Common.h"

#include <map>
#include <span>
#include <string>
#include <typeindex>
#include <vector>

#include <glm/glm.hpp>

namespace lake {

    class Entity;

    class EntityComponent {
    public:
        EntityComponent() = default;
        virtual ~EntityComponent() noexcept(false) = default;
    };

    class Entity {
    public:
        struct Properties {
            glm::vec3 position;
        };

        struct Registry {
            using entityFactory = Entity* (*)(const Properties& properties);
        
            Registry() = default;
            Registry(entityFactory factory, const std::string& identifier);
        
            static std::map<std::string, Registry>& getRegistry();

            entityFactory factory;
            std::string identifier;
        };

        template <typename T>
        struct RegisterEntity : public Registry {
            RegisterEntity(const std::string& identifier)
                : Registry([](const Properties& properties) -> Entity* {
                    return new T(properties);
                }, identifier)
            { }
        };

    public:
        Entity(const Properties& properties);
        virtual ~Entity();

        virtual void onUpdate(const f32 timeStep) { }

        template <typename T>
        void addComponent(T* component) requires std::is_base_of_v<EntityComponent, T> {
            mComponents[std::type_index(typeid(T))].push_back(component);
        }

        template <typename T>
        std::span<T*> getComponents() requires std::is_base_of_v<EntityComponent, T> {
            std::vector<lake::EntityComponent*>& components = mComponents[std::type_index(typeid(T))];

            return std::span<T*>(reinterpret_cast<T**>(components.data()), components.size());
        }

        template <typename T>
        bool hasComponent() requires std::is_base_of_v<EntityComponent, T> {
            return mComponents.find(std::type_index(typeid(T))) != mComponents.end();
        }

    private:
        std::map<std::type_index, std::vector<EntityComponent*>> mComponents;
    
    protected:
        glm::vec3 mPosition;
    };

} // namespace lake
