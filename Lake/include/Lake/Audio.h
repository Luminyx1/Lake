#pragma once

#include "Lake/Common.h"
#include "Lake/SoundComponent.h"

#include "Lake/PairUtils.h"

#include <fmod.hpp>

#include <span>
#include <unordered_map>
#include <tuple>

namespace lake {

    class Audio final {
    private:
        static constexpr int cMaxChannels = 1024;

    public:
        Audio();
        ~Audio();

        void update(std::span<SoundComponent*> soundComponents);

        void clearCache();

    private:
        FMOD::System* mSystem;
        std::unordered_map<std::pair<std::string, FMOD_MODE>, FMOD::Sound*, lake::PairHash, lake::PairEqual> mSounds;
    };

}
