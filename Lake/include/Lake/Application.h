#pragma once

#include "Lake/Common.h"
#include "Lake/Graphics.h"
#include "Lake/Scene.h"

namespace lake {
    
    extern int main(int argc, char** argv);

    class Application {
    private:
        friend int lake::main(int argc, char** argv);
    
    public:
        struct Properties {
            struct WindowProperties {
                u32 width, height;
            } window;
            std::string initialScene;
        };

    public:
        Application(const Properties& properties);
        virtual ~Application();
        
        virtual void onUpdate(const f32 timeStep) = 0;

    private:
        void run();
        void intermoduleDataTransfer();

    protected:
        Graphics mGraphics;
        Scene mScene;
    };


} // namespace lake
