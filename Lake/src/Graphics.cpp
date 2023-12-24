#include "Lake/Graphics.h"

#include "Lake/Log.h"
#include "Lake/Texture.h"
#include "Lake/ShaderProgram.h"
#include "Lake/Event.h"
#include "Lake/Application.h"
#include "Lake/PrimitiveShape.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

lake::Graphics::Graphics(const lake::Graphics::Properties& properties)
    : mTimeStep(0.0f)
    , mFrameTime(0.0f)
    , mLastFrameTime(0.0f)
    , mLayerStack(nullptr)
{
    bool success = glfwInit();
    LK_ASSERT(success, "Failed to initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(properties.window.width, properties.window.height, "Lake", nullptr, nullptr);

    LK_ASSERT(window != nullptr, "Failed to create GLFW window");

    glfwSetWindowIconifyCallback(window, [](GLFWwindow*, i32 iconified) {
        if (!iconified) {
            Application::raiseEvent(new lake::WindowMaximizeEvent());
        }
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, i32 width, i32 height) {
        if (width == 0 || height == 0) {
            Application::raiseEvent(new lake::WindowMinimizeEvent());
        } else {
            Application::raiseEvent(new lake::WindowResizeEvent(width, height));
        }
    });

    glfwSetKeyCallback(window, [](GLFWwindow*, i32 key, i32 scancode, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Application::raiseEvent(new lake::KeyPressEvent(key, scancode, mods));
        } else if (action == GLFW_RELEASE) {
            Application::raiseEvent(new lake::KeyReleaseEvent(key, scancode, mods));
        } else if (action == GLFW_REPEAT) {
            Application::raiseEvent(new lake::KeyRepeatEvent(key, scancode, mods));
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, i32 button, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Application::raiseEvent(new lake::MousePressEvent(button, mods));
        } else if (action == GLFW_RELEASE) {
            Application::raiseEvent(new lake::MouseReleaseEvent(button, mods));
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow*, f64 x, f64 y) {
        Application::raiseEvent(new lake::MouseMoveEvent(x, y));
    });

    glfwMakeContextCurrent(window);

    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    LK_ASSERT(success, "Failed to initialize GLAD");

#ifndef LK_DIST
    static const auto debugCallback = [](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:            return lake::error(std::string{"OpenGL: "} + message);
            case GL_DEBUG_SEVERITY_MEDIUM:          return lake::warn(std::string{"OpenGL: "} + message);
            case GL_DEBUG_SEVERITY_LOW:             return lake::info(std::string{"OpenGL: "} + message);
            case GL_DEBUG_SEVERITY_NOTIFICATION:    return lake::trace(std::string{"OpenGL: "} + message);
        }
    };

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
#endif

    PrimitiveShape::init();

    mLayerStack = new LayerStack({ properties.window.width, properties.window.height });
}

lake::Graphics::~Graphics() {
    Texture::clearCache();
    ShaderProgram::clearCache();

    delete mLayerStack;
    mLayerStack = nullptr;

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

    glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLayerStack->drawLayers();

    return !glfwWindowShouldClose(window);
}

void lake::Graphics::onEvent(Event* event) {
    if (event->getType() == lake::EventType::WindowResize) {
        const auto resizeEvent = static_cast<lake::WindowResizeEvent*>(event);

        mLayerStack->resize(resizeEvent->getSize());
    }
}

void lake::Graphics::pushDrawable(DrawableComponent* drawable, const std::size_t layerHash) {
    mLayerStack->pushDrawable(drawable, layerHash);
}

glm::u32vec2 lake::Graphics::getFramebufferSize() {
    glm::ivec2 size;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &size.x, &size.y);
    return size;
}
