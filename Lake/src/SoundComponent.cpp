#include "Lake/SoundComponent.h"

lake::SoundComponent::SoundComponent(const std::string& path, const LoopMode loopMode)
    : mPath(path)
    , mChannel(nullptr)
    , mMode(FMOD_DEFAULT | static_cast<FMOD_MODE>(loopMode))
    , mWantsToPlay(false)
{ }

lake::SoundComponent::~SoundComponent() {
    
}

void lake::SoundComponent::play() {
    if (mWantsToPlay) {
        return;
    }
    
    mWantsToPlay = true;
}

bool lake::SoundComponent::isPlaying() const {
    bool isPlaying = false;

    if (mChannel == nullptr || mChannel->isPlaying(&isPlaying) != FMOD_OK) {
        return false;
    }

    return isPlaying;
}
