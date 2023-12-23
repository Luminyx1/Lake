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
        SoundComponent(const std::string& path, const LoopMode loopMode = LoopMode::None, bool streamed = false);
        ~SoundComponent();

        void play();

        void setPitch(const f32 pitch) { mPitch = pitch; }
        void setVolume(const f32 volume) { mVolume = volume; }
        void setPaused(const bool paused) const;

        [[nodiscard]] bool isPlaying() const;
        [[nodiscard]] bool isPaused() const;
        [[nodiscard]] bool isStreamed() const { return mStreamed; }

        [[nodiscard]] const std::string& getPath() const { return mPath; } 
        [[nodiscard]] f32 getPitch() const { return mPitch; }
        [[nodiscard]] f32 getVolume() const { return mVolume; }

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
        bool mStreamed;
        f32 mPitch, mVolume;
    };

}
