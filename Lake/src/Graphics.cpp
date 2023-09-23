#include "Lake/Graphics.h"

#include "Lake/Log.h"
#include "Lake/Texture.h"
#include "Lake/ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

#ifndef LK_DIST
    static const auto debugCallback = [](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:            return lake::error(message);
            case GL_DEBUG_SEVERITY_MEDIUM:          return lake::warn(message);
            case GL_DEBUG_SEVERITY_LOW:             return lake::info(message);
            case GL_DEBUG_SEVERITY_NOTIFICATION:    return lake::trace(message);
        }
    };

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
#endif
}

lake::Graphics::~Graphics() {
    Texture::clearCache();
    ShaderProgram::clearCache();

    glfwDestroyWindow(glfwGetCurrentContext());
    glfwTerminate();
}

bool lake::Graphics::update() {
    GLFWwindow* const window = glfwGetCurrentContext();

    glfwPollEvents();
    glfwSwapBuffers(window);

    const f32 time = static_cast<f32>(glfwGetTime());
	mFrameTime = time - mLastFrameTime;
	mTimeStep = glm::min(mFrameTime, 0.0333f);
	mLastFrameTime = time;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLayerStack.drawLayers();

    return !glfwWindowShouldClose(window);
}

void lake::Graphics::pushDrawable(DrawableComponent* drawable, const std::size_t layerHash) {
    mLayerStack.pushDrawable(drawable, layerHash);
}

glm::u32vec2 lake::Graphics::getFramebufferSize() {
    glm::ivec2 size;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &size.x, &size.y);
    return size;
}
