#include "CustomRenderLayer.h"

void CustomRenderLayer::draw(const lake::RenderInfo& renderInfo) {
    // Clear the output framebuffer to white before drawing
    renderInfo.framebuffer->clear(glm::f32vec4{ 1.0f }, lake::Framebuffer::Type::Color);

    // We don't want to change any other behavior, so we'll just call the base draw function
    // This runs through all the drawables and draws them to the bound framebuffer
    lake::Layer::draw(renderInfo);
}
