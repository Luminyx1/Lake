#include "Lake/Entity.h"

#include "ChromaticAberrationComponent.h"
#include "FXAAComponent.h"

class PostProcessEffectEntity final : public lake::Entity {
    LK_REGISTER_ENTITY(PostProcessEffectEntity);

public:
    PostProcessEffectEntity(lake::Entity::Properties properties)
        : Entity()
    {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add a ChromaticAberrationComponent to apply a post-process effect.
        ChromaticAberrationComponent* chromaticAberrationComponent = new ChromaticAberrationComponent("pfx_chroma");
        this->addComponent<lake::DrawableComponent>(chromaticAberrationComponent);

        // Add an FXAAComponent to apply a post-process effect.
        FXAAComponent* fxaaComponent = new FXAAComponent("pfx_fxaa");
        this->addComponent<lake::DrawableComponent>(fxaaComponent);
    }

    ~PostProcessEffectEntity() override = default;

    void onEvent(lake::Event* event) override {
        //* Track events to resize the post-process effect.

        if (event->getType() == lake::EventType::WindowResize) {
            lake::WindowResizeEvent* e = static_cast<lake::WindowResizeEvent*>(event);

            ChromaticAberrationComponent* chromaticAberrationComponent = static_cast<ChromaticAberrationComponent*>(this->getComponents<lake::DrawableComponent>()[0]);
            chromaticAberrationComponent->resize(e->getSize());

            FXAAComponent* fxaaComponent = static_cast<FXAAComponent*>(this->getComponents<lake::DrawableComponent>()[1]);
            fxaaComponent->resize(e->getSize());
        }
    }
};
