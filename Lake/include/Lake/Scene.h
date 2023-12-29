#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"
#include "Lake/Event.h"

#include <string>
#include <string_view>

namespace lake {

    class Scene {
    public:
        Scene(const std::string& path);
        ~Scene();

        void update(const f32 timeStep);
        void onEvent(Event* event);

        void switchScene(const std::string& path);
        Entity* spawnEntity(const std::string_view type, const std::string& propertiesJson);

        const std::vector<Entity*>& getEntities() const { return mEntities; }

        [[nodiscard]] const std::string& getPath() const { return mPath; }

    private:
        Entity* spawnEntity(const std::string_view type, Entity::Properties& properties);
        void loadScene(const std::string& path);

        std::vector<Entity*> mEntities;
        std::string mPath;
    };

} // namespace lake
