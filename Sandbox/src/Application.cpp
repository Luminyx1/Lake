#include "Lake/EntryPoint.h"

#include "Lake/ImGuiLayer.h"

#include <imgui.h>

class SandboxApp final : public lake::Application {
public:
    SandboxApp(const lake::Application::Properties& properties)
        : lake::Application(properties)
    {
        mGraphics.getLayerStack().pushLayer<lake::ImGuiLayer>("ImGui");

        extern void setupImGuiStyle();
        setupImGuiStyle();
    }

    void onUpdate(const f32 ts) override {
        ImGui::ShowDemoWindow();
    }
};

lake::Application* lake::createApplication() {
    return new SandboxApp({
        .window = {
            .width = 1920,
            .height = 1080
        },
        .initialScene = "scene.json"
    });
}
