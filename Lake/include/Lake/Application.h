#pragma once

#include "Lake/Common.h"
#include "Lake/Event.h"
#include "Lake/Graphics.h"
#include "Lake/Physics.h"
#include "Lake/Scene.h"

#include <queue>

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
        virtual void onEvent(Event* event) { }

        static void raiseEvent(Event* event);

    private:
        void run();
        void handleEvents();
        void intermoduleDataTransfer();

        static std::deque<Event*> sEventQueue;

    protected:
        Graphics mGraphics;
        Scene mScene;
        Physics mPhysics;
    };


} // namespace lake
