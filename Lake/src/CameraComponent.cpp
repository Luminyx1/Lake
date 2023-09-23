#include "Lake/CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

lake::CameraComponent::CameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up)
    : EntityComponent()
    , mPosition(position)
    , mTargetLayerHash(0)
    , mIsDirty(true)
{
    this->setView(position, lookTarget, up);
}

void lake::CameraComponent::setView(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up) {
    mView = glm::lookAt(position, lookTarget, up);
    
    mPosition = position;
}

lake::OrthographicCameraComponent::OrthographicCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far)
    : CameraComponent(position, lookTarget, up)
{
    this->setProjection(top, bottom, left, right, near, far);
}

void lake::OrthographicCameraComponent::setProjection(const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far) {
    mProjection = glm::ortho(left, right, bottom, top, near, far);

    mIsDirty = true;
}
