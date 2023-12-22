#pragma once

#include "Lake/Common.h"
#include "Lake/SoundComponent.h"

#include <fmod.hpp>

#include <span>

namespace lake {

    class Audio final {
    private:
        static constexpr int cMaxChannels = 1024;

    public:
        Audio();
        ~Audio();

        void update(std::span<SoundComponent*> soundComponents);

    private:
        FMOD::System* mSystem;
    };

}
