#include "Lake/Entity.h"
#include "Lake/CameraComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"

class MainCamera final : public lake::Entity {
    //* Register our class so we can create it from a scene file.
    LK_REGISTER_ENTITY(MainCamera);

public:
    MainCamera(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mLookTarget(0.0f, 0.0f, 1.0f)
    { }

    ~MainCamera() override = default;

    void onCreate() override {
        //* Add components to build our entity. Then, control the entity in onUpdate.

        // Add an orthographic camera component to render the scene with.
        const f32 aspectRatio = static_cast<f32>(lake::Graphics::getFramebufferSize().x) / static_cast<f32>(lake::Graphics::getFramebufferSize().y);
        lake::OrthographicCameraComponent* camera = new lake::OrthographicCameraComponent(
            mPosition,
            mLookTarget,
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f, -1.0f, aspectRatio, -aspectRatio, -1.0f, 1.0f
        );
        camera->setTargetLayer("main"); // Assign the camera to the "main" layer.
        this->addComponent<lake::CameraComponent>(camera);
    }

    void onUpdate(const f32 timeStep) override {
        //* Act behaviour for our entity. This is called every frame.
        
        // Update the camera component's view matrix. We use a for loop here because we can have multiple camera components, however in this case we only have one.
        std::span<lake::CameraComponent*> cameraComponents = this->getComponents<lake::CameraComponent>();
        for (auto& component : cameraComponents) {
            component->setView(mPosition, mLookTarget);
        }
    }

    void onEvent(lake::Event* event) override {
        //* Track events to correct for screen size and aspect ratio changes during a resize.

        // Update the projection matrix on window resize.
        if (event->getType() == lake::EventType::WindowResize) {
            const auto resizeEvent = static_cast<lake::WindowResizeEvent*>(event);
            const f32 aspectRatio = static_cast<f32>(resizeEvent->getSize().x) / static_cast<f32>(resizeEvent->getSize().y);

            std::span<lake::CameraComponent*> cameraComponents = this->getComponents<lake::CameraComponent>();
            for (auto component : cameraComponents) {
                if (lake::OrthographicCameraComponent* camera = dynamic_cast<lake::OrthographicCameraComponent*>(component)) {
                    camera->setProjection(1.0f, -1.0f, aspectRatio, -aspectRatio, -1.0f, 1.0f);
                }
            }
        }
    }

private:
    glm::vec3 mPosition, mLookTarget;
};
