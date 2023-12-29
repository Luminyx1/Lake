#include "Lake/Entity.h"

#include "Lake/ChromaticAberrationComponent.h"
#include "Lake/FXAAComponent.h"
#include "BackgroundRendererComponent.h"

class PostProcessEffectEntity final : public lake::Entity {
    LK_REGISTER_ENTITY(PostProcessEffectEntity);

public:
    PostProcessEffectEntity(lake::Entity::Properties properties)
        : Entity()
    { }

    ~PostProcessEffectEntity() override = default;

    void onCreate() override {
        this->addComponent<lake::DrawableComponent>(new BackgroundRendererComponent("background"));
        this->addComponent<lake::DrawableComponent>(new lake::ChromaticAberrationComponent("pfx_chroma"));
        this->addComponent<lake::DrawableComponent>(new lake::FXAAComponent("pfx_fxaa"));
    }

    void onUpdate(f32 timeStep) override {
        BackgroundRendererComponent* bgRenderer = static_cast<BackgroundRendererComponent*>(this->getComponents<lake::DrawableComponent>()[0]);
        bgRenderer->update(timeStep);
    }
};
