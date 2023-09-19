#include "Lake/Application.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>

Lake::Application::Application(const Lake::Application::Properties& properties)
    : mTimeStep(0.0f)
    , mFrameTime(0.0f)
    , mLastFrameTime(0.0f)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(properties.window.width, properties.window.height, "Lake", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

Lake::Application::~Application() {
    glfwDestroyWindow(glfwGetCurrentContext());
    glfwTerminate();
}

void Lake::Application::run() {
    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        glfwPollEvents();

        this->OnUpdate(mTimeStep);

        glfwSwapBuffers(glfwGetCurrentContext());

        f32 time = static_cast<f32>(glfwGetTime());
		mFrameTime = time - mLastFrameTime;
		mTimeStep = glm::min(mFrameTime, 0.0333f);
		mLastFrameTime = time;
    }
}
