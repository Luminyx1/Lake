#include "Lake/EntryPoint.h"

#include "Lake/ImGuiLayer.h"
#include "Lake/Log.h"

#include <imgui.h>

class SandboxApp final : public lake::Application {
public:
    SandboxApp(const lake::Application::Properties& properties)
        : lake::Application(properties)
    {
        mGraphics.getLayerStack().pushLayer<lake::Layer>("main");
        mGraphics.getLayerStack().pushLayer<lake::ImGuiLayer>("ImGui");

        extern void setupImGuiStyle();
        setupImGuiStyle();
    }

    void onUpdate(const f32 timeStep) override {
        if (ImGui::Begin("Panel")) {
            ImGui::Text("FPS: %f", 1.0f / timeStep);
        } ImGui::End();
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
