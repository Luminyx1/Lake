#include "Lake/EntryPoint.h"

class SandboxApp final : public lake::Application {
public:
    SandboxApp(const lake::Application::Properties& properties)
        : lake::Application(properties)
    {
        //* Initialize the environment for our application. This is called before the application loop starts.

        // Push layers to the layer stack. The order of layers is important, as they are called in order from top to bottom.
        mGraphics.getLayerStack().pushLayer<lake::Layer>("background"); // Render the background scene below the main scene.
        mGraphics.getLayerStack().pushLayer<lake::Layer>("main");       // Render the main scene.
        mGraphics.getLayerStack().pushLayer<lake::Layer>("pfx_chroma"); // Render the chromatic aberration post-process effect after the main scene has been rendered.
        mGraphics.getLayerStack().pushLayer<lake::Layer>("pfx_fxaa");   // Render the FXAA post-process effect after the chromatic aberration effect has been rendered.
    }

    void onUpdate(const f32 timeStep) override { }
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
