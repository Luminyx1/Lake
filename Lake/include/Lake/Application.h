#pragma once

#include "Lake/Common.h"

namespace Lake {
    
    extern int lmain(int argc, char** argv);

    class LK_API Application {
    private:
        friend int Lake::lmain(int argc, char** argv);
    
    public:
        struct Properties {
            struct {
                u32 width, height;
            } window;
        };

    public:
        Application(const Properties& properties);
        virtual ~Application();
        
        virtual void OnUpdate(const f32 ts) = 0;

    private:
        void run();

        f32 mTimeStep, mFrameTime, mLastFrameTime;
    };


} // namespace Lake
