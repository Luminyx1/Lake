#include "Lake/ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

lake::ImGuiLayer::ImGuiLayer(const std::string& name)
    : Layer(name)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= (ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

lake::ImGuiLayer::~ImGuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void lake::ImGuiLayer::draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupCurrentContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
