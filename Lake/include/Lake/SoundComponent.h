#pragma once

#include "Lake/Common.h"
#include "Lake/Entity.h"
#include "Lake/Log.h"

#include <fmod.hpp>

#include <string>

namespace lake {

    class SoundComponent : public EntityComponent {
    public:
        enum class LoopMode {
            None,
            LoopFromStart = FMOD_LOOP_NORMAL,
            LoopBackAndForth = FMOD_LOOP_BIDI
        };
    
    public:
        SoundComponent(const std::string& path, const LoopMode loopMode = LoopMode::None);
        ~SoundComponent();

        void play();

        [[nodiscard]] bool isPlaying() const;

        [[nodiscard]] const std::string& getPath() const { return mPath; } 

    private:
        friend class Audio;

        [[nodiscard]] FMOD_MODE getMode() const { return mMode; }
        [[nodiscard]] bool getWantsToPlay() const { return mWantsToPlay; }

        void setChannel(FMOD::Channel* channel) { mChannel = channel; }
        void setWantsToPlay(const bool wantsToPlay) { mWantsToPlay = wantsToPlay; }

        std::string mPath;
        FMOD::Channel* mChannel;
        FMOD_MODE mMode;
        bool mWantsToPlay;
    };

}
