#include "Lake/Application.h"

#include "Lake/Log.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <cmath>

Lake::Application::Application(const Lake::Application::Properties& properties)
    : mTimeStep(0.0f)
    , mFrameTime(0.0f)
    , mLastFrameTime(0.0f)
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= (ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

Lake::Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glfwGetCurrentContext());
    glfwTerminate();
}

void Lake::Application::run() {
    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        this->OnUpdate(mTimeStep);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);

        glfwSwapBuffers(glfwGetCurrentContext());

        const f32 time = static_cast<f32>(glfwGetTime());
		mFrameTime = time - mLastFrameTime;
		mTimeStep = glm::min(mFrameTime, 0.0333f);
		mLastFrameTime = time;
    }
}
