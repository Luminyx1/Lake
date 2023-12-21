#pragma once

#include "Lake/Common.h"

#include "Lake/Entity.h"

#include <glm/glm.hpp>

#include <string>

namespace lake {

    class TagComponent : public EntityComponent {
    public:
        TagComponent(const std::string& tag)
            : mTag(tag)
        { }
        
        ~TagComponent() override = default;

        [[nodiscard]] const std::string& getTag() const { return mTag; }

    private:
        std::string mTag;
    };

} // namespace lake
