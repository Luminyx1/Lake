#pragma once

#include "Lake/Common.h"
#include "Lake/Entity.h"
#include "Lake/Log.h"

#include <fmod.hpp>

#include <string>

namespace lake {

    class SoundComponent : public EntityComponent {
    public:
        SoundComponent(const std::string& path);
        ~SoundComponent();

        void play();

        [[nodiscard]] bool isPlaying() const {
            bool isPlaying = false;

            if (mChannel == nullptr || mChannel->isPlaying(&isPlaying) != FMOD_OK) {
                return false;
            }

            return isPlaying;
        }

        [[nodiscard]] const std::string& getPath() const { return mPath; } 

    private:
        friend class Audio;

        std::string mPath;
        FMOD::Channel* mChannel;
        bool mWantsToPlay;
    };

}
