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
        virtual ~EntityComponent() = default;
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

        virtual void onUpdate(const f32 ts) { }

        template <typename T>
        void addComponent(T* component) {
            static_assert(std::is_base_of_v<EntityComponent, T>, "T must derive from EntityComponent");

            mComponents[std::type_index(typeid(T))].push_back(component);
        }

        template <typename T>
        void addComponent() {
            static_assert(std::is_base_of_v<EntityComponent, T>, "T must derive from EntityComponent");

            mComponents[std::type_index(typeid(T))].push_back(new T());
        }

        template <typename T>
        std::span<T*> getComponents() {
            static_assert(std::is_base_of_v<EntityComponent, T>, "T must derive from EntityComponent");

            auto& components = mComponents[std::type_index(typeid(T))];

            return std::span<T*>(reinterpret_cast<T**>(components.data()), components.size());
        }

        template <typename T>
        bool hasComponent() {
            static_assert(std::is_base_of_v<EntityComponent, T>, "T must derive from EntityComponent");

            return mComponents.find(std::type_index(typeid(T))) != mComponents.end();
        }

    private:
        std::map<std::type_index, std::vector<EntityComponent*>> mComponents;
    
    protected:
        glm::vec3 mPosition;
    };

} // namespace lake
