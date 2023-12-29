#include "Lake/Audio.h"

#include "Lake/Log.h"

#include <fmod_errors.h>

#ifdef LK_PLATFORM_WINDOWS
#undef APIENTRY
#include <Windows.h>
#endif

lake::Audio::Audio()
    : mSystem(nullptr)
{
#ifdef LK_PLATFORM_WINDOWS
    {
        HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        LK_ASSERT(result == S_OK, "Failed to initialize COM: ", result);
    }
#endif

    FMOD_RESULT result = FMOD::System_Create(&mSystem);
    LK_ASSERT(result == FMOD_OK, "Failed to create FMOD system: ", FMOD_ErrorString(result));

    result = mSystem->init(Audio::cMaxChannels, FMOD_INIT_NORMAL, nullptr);
    LK_ASSERT(result == FMOD_OK, "Failed to initialize FMOD system: ", FMOD_ErrorString(result));

#ifndef LK_DIST
    #ifdef LK_COMPILER_MSVC
    #pragma warning(push)
    #pragma warning(disable: 4311) // pointer truncation
    #pragma warning(disable: 4302) // truncation
    #endif

    static const auto debugCallback = [](FMOD_SYSTEM* system, FMOD_SYSTEM_CALLBACK_TYPE type, void* commanddata1, void* commanddata2, void* userdata) -> FMOD_RESULT {
        if (type & FMOD_SYSTEM_CALLBACK_DEVICELISTCHANGED) {
            lake::trace("FMOD device list changed");
        }
        if (type & FMOD_SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED) {
            lake::error("FMOD memory allocation failed at ", (const char*)commanddata1, ", requested ", reinterpret_cast<int>(commanddata2), " bytes");
        }
        if (type & FMOD_SYSTEM_CALLBACK_THREADCREATED) {
            lake::trace("FMOD thread created \"", (const char*)commanddata2, "\"");
        }
        if (type & FMOD_SYSTEM_CALLBACK_BADDSPCONNECTION) {
            lake::error("FMOD bad DSP connection");
        }
        if (type & FMOD_SYSTEM_CALLBACK_PREMIX) {
            lake::trace("FMOD premix");
        }
        if (type & FMOD_SYSTEM_CALLBACK_POSTMIX) {
            lake::trace("FMOD postmix");
        }
        if (type & FMOD_SYSTEM_CALLBACK_ERROR) {
            FMOD_ERRORCALLBACK_INFO* info = static_cast<FMOD_ERRORCALLBACK_INFO*>(commanddata1);
            lake::error("FMOD: ", FMOD_ErrorString(info->result));
        }
        if (type & FMOD_SYSTEM_CALLBACK_MIDMIX) {
            lake::trace("FMOD midmix");
        }
        if (type & FMOD_SYSTEM_CALLBACK_THREADDESTROYED) {
            lake::trace("FMOD thread destroyed \"", (const char*)commanddata2, "\"");
        }
        if (type & FMOD_SYSTEM_CALLBACK_PREUPDATE) {
            lake::trace("FMOD preupdate");
        }
        if (type & FMOD_SYSTEM_CALLBACK_POSTUPDATE) {
            lake::trace("FMOD postupdate");
        }
        if (type & FMOD_SYSTEM_CALLBACK_RECORDLISTCHANGED) {
            lake::trace("FMOD record list changed");
        }
        if (type & FMOD_SYSTEM_CALLBACK_BUFFEREDNOMIX) {
            lake::trace("FMOD buffered no mix");
        }
        if (type & FMOD_SYSTEM_CALLBACK_DEVICEREINITIALIZE) {
            lake::trace("FMOD device reinitialize");
        }
        if (type & FMOD_SYSTEM_CALLBACK_OUTPUTUNDERRUN) {
            lake::trace("FMOD output underrun");
        }
        if (type & FMOD_SYSTEM_CALLBACK_RECORDPOSITIONCHANGED) {
            lake::trace("FMOD record position changed on sound to: ", reinterpret_cast<int>(commanddata2));
        }

        return FMOD_OK;
    };

    #ifdef LK_COMPILER_MSVC
    #pragma warning(pop)
    #endif

    FMOD_SYSTEM_CALLBACK_TYPE callbackMask = FMOD_SYSTEM_CALLBACK_ALL;

    // Disable spammy callbacks
    callbackMask &= ~(FMOD_SYSTEM_CALLBACK_PREUPDATE | FMOD_SYSTEM_CALLBACK_POSTUPDATE);
    callbackMask &= ~(FMOD_SYSTEM_CALLBACK_PREMIX | FMOD_SYSTEM_CALLBACK_POSTMIX | FMOD_SYSTEM_CALLBACK_MIDMIX);
    callbackMask &= ~(FMOD_SYSTEM_CALLBACK_RECORDLISTCHANGED);

    mSystem->setCallback(debugCallback, callbackMask);
#endif

    lake::trace("FMOD system initialized");
}

lake::Audio::~Audio() {
    mSystem->release();
    lake::trace("FMOD system shut down");

#ifdef LK_PLATFORM_WINDOWS
    CoUninitialize();
#endif
}

void lake::Audio::update(std::span<SoundComponent*> soundComponents) {
    static const auto playSound = [this](FMOD::Sound* sound, SoundComponent* soundComponent) {
        FMOD::Channel* channel = nullptr;
        mSystem->playSound(sound, nullptr, false, &channel);
        channel->setPitch(soundComponent->getPitch());
        channel->setVolume(soundComponent->getVolume());
        soundComponent->setChannel(channel);
        soundComponent->setWantsToPlay(false);
    };

    for (SoundComponent* soundComponent : soundComponents) {
        if (soundComponent->getWantsToPlay()) {
            // First check our cache of loaded sounds
            //? TODO: Do we need to cache streamed sounds?
            const auto it = mSounds.find(std::make_pair(soundComponent->getPath(), soundComponent->getMode()));
            if (it != mSounds.end()) {
                const auto sound = it->second;

                playSound(sound, soundComponent);
                continue;
            }

            // If this sound has already been loaded with the same mode, just play it
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = soundComponent->getMode();
            mode |= FMOD_LOWMEM;
            if (soundComponent->isStreamed()) {
                mode |= FMOD_CREATESTREAM;
                if (soundComponent->getMode() & FMOD_LOOP_BIDI) {
                    lake::warn("Streamed sounds cannot be played in LoopBackAndForth mode, defaulting to LoopFromStart");
                    mode &= ~FMOD_LOOP_BIDI;
                    mode |= FMOD_LOOP_NORMAL;
                }
            }
            mSystem->createSound(soundComponent->getPath().c_str(), mode, nullptr, &sound);

            LK_ASSERT(sound != nullptr, "Failed to load sound: ", soundComponent->getPath());

            lake::trace("Loading sound: ", soundComponent->getPath());
            mSounds[std::make_pair(soundComponent->getPath(), soundComponent->getMode())] = sound;

            playSound(sound, soundComponent);
        }
    }

    mSystem->update();
}

void lake::Audio::clearCache() {
    for (auto& [key, sound] : mSounds) {
        sound->release();
    }

    mSounds.clear();
}
