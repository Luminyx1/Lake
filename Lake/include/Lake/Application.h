#pragma once

#include "Lake/Common.h"
#include "Lake/Graphics.h"

namespace lake {
    
    extern int main(int argc, char** argv);

    class LK_API Application {
    private:
        friend int lake::main(int argc, char** argv);
    
    public:
        struct Properties {
            struct WindowProperties {
                u32 width, height;
            } window;
        };

    public:
        Application(const Properties& properties);
        virtual ~Application();
        
        virtual void onUpdate(const f32 ts) = 0;

    private:
        void run();

    protected:
        Graphics mGraphics;
    };


} // namespace lake
