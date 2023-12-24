#include "Lake/EntryPoint.h"

#include "Lake/ImGuiLayer.h"
#include "Lake/Log.h"

#include <imgui.h>

#include "CustomRenderLayer.h"

class SandboxApp final : public lake::Application {
public:
    SandboxApp(const lake::Application::Properties& properties)
        : lake::Application(properties)
    {
        //* Initialize the environment for our application. This is called before the application loop starts.

        // Push layers to the layer stack. The order of layers is important, as they are called in order from top to bottom.
        mGraphics.getLayerStack().pushLayer<CustomRenderLayer>("main"); // Render the main scene. We use a custom layer here to render our scene on a white background instead of the default black.
        mGraphics.getLayerStack().pushLayer<lake::ImGuiLayer>("ImGui"); // Render ImGui overlay.

        // Additional setup such as setting the ImGui style.
        extern void setupImGuiStyle();
        setupImGuiStyle();
    }

    void onUpdate(const f32 timeStep) override {
        //* Global update function. Called every frame.

        // Display an informational panel.
        if (ImGui::Begin("Panel")) {
            ImGui::Text("FPS: %f", 1.0f / timeStep);
        } ImGui::End();
    }
};

lake::Application* lake::createApplication() {
    //* Create the application with our desired params and return it. This is the entry point for the engine.

    return new SandboxApp({
        .window = {
            .width = 1920,
            .height = 1080
        },
        .initialScene = "scene.json"
    });
}
