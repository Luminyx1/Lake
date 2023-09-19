#include "Lake/EntryPoint.h"

class SandboxApp : public Lake::Application {
public:
    SandboxApp(const Lake::Application::Properties& properties)
        : Lake::Application(properties)
    { }

    void OnUpdate(const f32 ts) override {

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
