#include "Lake/Entity.h"
#include "Lake/SoundComponent.h"
#include "Lake/TagComponent.h"

class MusicPlayer final : public lake::Entity {
    LK_REGISTER_ENTITY(MusicPlayer);

public:
    MusicPlayer(lake::Entity::Properties properties)
        : Entity()
    { }

    ~MusicPlayer() override = default;

    void onCreate() override {
        lake::SoundComponent* soundComponent = new lake::SoundComponent("massobeats - honeyjam.mp3", lake::SoundComponent::LoopMode::LoopFromStart, true);
        this->addComponent<lake::SoundComponent>(soundComponent);
        soundComponent->play();
    }
};
