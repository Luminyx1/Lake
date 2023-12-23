#include "Lake/SoundComponent.h"

lake::SoundComponent::SoundComponent(const std::string& path, const LoopMode loopMode, bool streamed)
    : mPath(path)
    , mChannel(nullptr)
    , mMode(static_cast<FMOD_MODE>(loopMode))
    , mWantsToPlay(false)
    , mStreamed(streamed)
    , mPitch(1.0f)
    , mVolume(1.0f)
{ }

void lake::SoundComponent::play() {
    if (mWantsToPlay) {
        return;
    }
    
    mWantsToPlay = true;
}

void lake::SoundComponent::setPaused(const bool paused) const {
    if (mChannel == nullptr) {
        return;
    }

    mChannel->setPaused(paused);
}

bool lake::SoundComponent::isPlaying() const {
    bool isPlaying = false;

    if (mChannel == nullptr || mChannel->isPlaying(&isPlaying) != FMOD_OK) {
        return false;
    }

    return isPlaying;
}

bool lake::SoundComponent::isPaused() const {
    bool isPaused = false;

    if (mChannel == nullptr || mChannel->getPaused(&isPaused) != FMOD_OK) {
        return false;
    }

    return isPaused;
}
