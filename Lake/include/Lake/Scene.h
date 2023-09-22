#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"

#include <string>

namespace lake {

    class LK_API Scene {
    public:
        Scene(const std::string& path);
        ~Scene();

        void update(const f32 ts);

    private:
        std::vector<Entity*> mEntities;
    };

} // namespace lake
