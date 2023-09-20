#include "Lake/Graphics.h"

#include "Lake/Log.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

lake::Graphics::Graphics(const lake::Graphics::Properties& properties)
    : mTimeStep(0.0f)
    , mFrameTime(0.0f)
    , mLastFrameTime(0.0f)
    , mLayerStack()
{
    bool success = glfwInit();
    LK_ASSERT(success, "Failed to initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(properties.window.width, properties.window.height, "Lake", nullptr, nullptr);

    LK_ASSERT(window != nullptr, "Failed to create GLFW window");

    glfwMakeContextCurrent(window);

    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    LK_ASSERT(success, "Failed to initialize GLAD");
}

lake::Graphics::~Graphics() {
    glfwDestroyWindow(glfwGetCurrentContext());
    glfwTerminate();
}

bool lake::Graphics::update() {
    glfwPollEvents();
    glfwSwapBuffers(glfwGetCurrentContext());

    const f32 time = static_cast<f32>(glfwGetTime());
	mFrameTime = time - mLastFrameTime;
	mTimeStep = glm::min(mFrameTime, 0.0333f);
	mLastFrameTime = time;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLayerStack.drawLayers();

    return !glfwWindowShouldClose(glfwGetCurrentContext());
}

void lake::Graphics::pushDrawable(Drawable* drawable, const std::size_t layerHash) {
    mLayerStack.pushDrawable(drawable, layerHash);
}
