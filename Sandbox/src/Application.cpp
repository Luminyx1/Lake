#include "Lake/EntryPoint.h"

#include "Lake/ImGuiLayer.h"
#include "Lake/Log.h"

#include <imgui.h>

class RenderLayer final : public lake::Layer {
public:
    RenderLayer(const std::string& name)
        : Layer(name)
    { }

    void draw() override {
        lake::RenderInfo renderInfo = {
            .camera = mCamera
        };

        for (auto& drawable : mDrawables) {
            drawable->draw(renderInfo);
        }
    }
};

class SandboxApp final : public lake::Application {
public:
    SandboxApp(const lake::Application::Properties& properties)
        : lake::Application(properties)
    {
        mGraphics.getLayerStack().pushLayer<lake::ImGuiLayer>("ImGui");
        mGraphics.getLayerStack().pushLayer<RenderLayer>("main");

        extern void setupImGuiStyle();
        setupImGuiStyle();
    }

    void onUpdate(const f32 timeStep) override {
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
