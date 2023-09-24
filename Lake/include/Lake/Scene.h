#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"
#include "Lake/Event.h"

#include <string>

namespace lake {

    class Scene {
    public:
        Scene(const std::string& path);
        ~Scene();

        void update(const f32 timeStep);
        void onEvent(Event* event);

        const std::vector<Entity*>& getEntities() const { return mEntities; }

    private:
        std::vector<Entity*> mEntities;
    };

} // namespace lake
