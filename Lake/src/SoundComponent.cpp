#include "Lake/SoundComponent.h"

lake::SoundComponent::SoundComponent(const std::string& path)
    : mPath(path)
    , mChannel(nullptr)
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
