#include "Lake/Application.h"

#include "Lake/Log.h"

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

lake::Application::Application(const lake::Application::Properties& properties)
    : mGraphics({
        .window = {
            .width = properties.window.width,
            .height = properties.window.height
        }
    })
{ }

lake::Application::~Application() {

}

void lake::Application::run() {
    while (mGraphics.update()) {
        this->onUpdate(mGraphics.getTimeStep());
    }
}
