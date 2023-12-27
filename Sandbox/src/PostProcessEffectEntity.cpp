#include "Lake/Entity.h"

#include "Lake/ChromaticAberrationComponent.h"
#include "Lake/FXAAComponent.h"

class PostProcessEffectEntity final : public lake::Entity {
    LK_REGISTER_ENTITY(PostProcessEffectEntity);

public:
    PostProcessEffectEntity(lake::Entity::Properties properties)
        : Entity()
    {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add a ChromaticAberrationComponent to apply a post-process effect.
        this->addComponent<lake::DrawableComponent>(new lake::ChromaticAberrationComponent("pfx_chroma"));

        // Add an FXAAComponent to apply a post-process effect.
        this->addComponent<lake::DrawableComponent>(new lake::FXAAComponent("pfx_fxaa"));
    }

    ~PostProcessEffectEntity() override = default;
};
