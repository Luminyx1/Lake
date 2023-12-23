#include "Lake/Entity.h"
#include "Lake/SoundComponent.h"

class MusicPlayer final : public lake::Entity {
    LK_REGISTER_ENTITY(MusicPlayer);

public:
    MusicPlayer(lake::Entity::Properties properties)
        : Entity()
    {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add a SoundComponent to play our background music.
        lake::SoundComponent* soundComponent = new lake::SoundComponent("massobeats - honeyjam.mp3", lake::SoundComponent::LoopMode::LoopFromStart, true);
        this->addComponent<lake::SoundComponent>(soundComponent);

        // Get some properties from the scene file.
        const f32 volume = static_cast<f32>(properties.value()["volume"].get<f64>());
        const f32 pitch = static_cast<f32>(properties.value()["pitch"].get<f64>());
        soundComponent->setVolume(volume);
        soundComponent->setPitch(pitch);

        soundComponent->play();
    }

    ~MusicPlayer() override = default;

    void onEvent(lake::Event* event) override {
        //* Track events to pause and unpause the music.

        if (event->getType() == lake::EventType::KeyPress) {
            lake::KeyPressEvent* e = static_cast<lake::KeyPressEvent*>(event);
            if (e->getKey() == GLFW_KEY_SPACE) {
                lake::SoundComponent* soundComponent = this->getComponents<lake::SoundComponent>()[0];
                soundComponent->setPaused(!soundComponent->isPaused());
            }
        }
    }
};
