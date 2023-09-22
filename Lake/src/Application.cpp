#include "Lake/Application.h"

#include "Lake/Log.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

lake::Application::Application(const lake::Application::Properties& properties)
    : mGraphics({
        .window = {
            .width = properties.window.width,
            .height = properties.window.height
        }
    })
    , mScene(properties.initialScene)
{ }

lake::Application::~Application() {

}

void lake::Application::run() {
    while (mGraphics.update()) {
        const f32 ts = mGraphics.getTimeStep();

        this->onUpdate(ts);

        mScene.update(ts);
    }
}
