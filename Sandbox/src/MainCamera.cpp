#include "Lake/Entity.h"
#include "Lake/CameraComponent.h"
#include "Lake/Graphics.h"
#include "Lake/JsonHelpers.h"

class MainCamera final : public lake::Entity {
public:
    MainCamera(lake::Entity::Properties properties)
        : Entity()
        , mPosition(lake::json::getVec3(properties, "position"))
        , mLookTarget(0.0f, 0.0f, 1.0f)
    {
        const f32 aspectRatio = static_cast<f32>(lake::Graphics::getFramebufferSize().x) / static_cast<f32>(lake::Graphics::getFramebufferSize().y);
        lake::OrthographicCameraComponent* camera = new lake::OrthographicCameraComponent(
            mPosition,
            mLookTarget,
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f, -1.0f, aspectRatio, -aspectRatio, -1.0f, 1.0f
        );
        camera->setTargetLayer("main");
        this->addComponent<lake::CameraComponent>(camera);
    }

    ~MainCamera() override = default;

    void onUpdate(const f32 timeStep) override {
        std::span<lake::CameraComponent*> cameraComponents = this->getComponents<lake::CameraComponent>();
        for (auto& component : cameraComponents) {
            component->setView(mPosition, mLookTarget);
        }
    }

private:
    glm::vec3 mPosition, mLookTarget;
};

lake::Entity::RegisterEntity<MainCamera> mainCamera("MainCamera");
