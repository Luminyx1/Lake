#include "Lake/EntryPoint.h"

#include <imgui.h>

class SandboxApp : public Lake::Application {
public:
    SandboxApp(const Lake::Application::Properties& properties)
        : Lake::Application(properties)
    {
        extern void SetupImGuiStyle();

        SetupImGuiStyle();
    }

    void OnUpdate(const f32 ts) override {
        ImGui::ShowDemoWindow();
    }
};

Lake::Application* Lake::createApplication() {
    return new SandboxApp({
        .window = {
            .width = 1920,
            .height = 1080
        }
    });
}
