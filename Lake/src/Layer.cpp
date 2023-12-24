#include "Lake/Layer.h"

#include "Lake/Log.h"
#include "Lake/PrimitiveShape.h"

lake::Layer::Layer(const std::string& name)
    : mDrawables()
    , mName(name)
    , mCamera(nullptr)
    , mGraphicsContext()
{
    mGraphicsContext
        .depth(GraphicsContext::DepthFunction::LessEqual, true)
    ;
}

void lake::Layer::draw(const lake::RenderInfo& renderInfo) {
    mGraphicsContext.apply();

    for (auto& drawable : mDrawables) {
        drawable->draw(renderInfo);
    }
}

lake::LayerStack::LayerStack(const glm::u32vec2& size)
    : mLayers()
    , mFramebuffer(size)
    , mCompositorShader("lake/assets/shaders/compositor.vsh", "lake/assets/shaders/compositor.fsh")
    , mGraphicsContext()
{
    mFramebuffer.addTextureBuffer(Texture::Format::RGBA16F); //? Is this the right format?
    mFramebuffer.finalize();

    mGraphicsContext
        .blend(false)
        .cull(false)
        .depth(false)
    ;
}

lake::LayerStack::~LayerStack() {
    this->clearLayers();
}

void lake::LayerStack::popLayer() {
    if (!mLayers.empty()) {
        delete mLayers.back().second;
        mLayers.pop_back();
    }
}

void lake::LayerStack::removeLayer(const std::string& name) {
    const std::size_t targetHash = std::hash<std::string>{}(name);

    auto it = this->getLayerIterator(targetHash);

    if (it != mLayers.end()) {
        delete it->second;
        mLayers.erase(it);
    } else {
        lake::warn("Unable to remove nonexistent layer: ", name);
    }
}

void lake::LayerStack::clearLayers() {
    for (auto& [hash, layer] : mLayers) {
        delete layer;
    }

    mLayers.clear();
}

lake::Layer* lake::LayerStack::getLayer(const std::size_t hash) {
    auto it = this->getLayerIterator(hash);

    if (it != mLayers.end()) {
        return it->second;
    }

    return nullptr;
}

void lake::LayerStack::resize(const glm::u32vec2& size) {
    glViewport(0, 0, size.x, size.y);

    for (auto& [hash, layer] : mLayers) {
        layer->resize(size);
    }
}

void lake::LayerStack::pushDrawable(DrawableComponent* drawable, const std::size_t layerHash) {
    auto it = this->getLayerIterator(layerHash);

    if (it != mLayers.end()) {
        it->second->mDrawables.push_back(drawable);
    } else {
        lake::warn("Unable to push drawable to nonexistent layer: ", layerHash);
    }
}

void lake::LayerStack::drawLayers() const {
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mFramebuffer.bind();
    mFramebuffer.clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    for (const auto& [hash, layer] : mLayers) {
        const RenderInfo renderInfo = {
            .camera = layer->getCamera(),
            .framebuffer = &mFramebuffer,
        };

        layer->draw(renderInfo);
        layer->mDrawables.clear();
    }

    mGraphicsContext.apply();

    Framebuffer::getBackbuffer()->bind();

    glBindVertexArray(PrimitiveShape::getQuadVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PrimitiveShape::getQuadEBO());

    mCompositorShader.bind();
    mFramebuffer.getTextureBuffer(0)->bind(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

lake::LayerStack::LayerContainer::iterator lake::LayerStack::getLayerIterator(const std::size_t hash) {
    return std::find_if(mLayers.begin(), mLayers.end(), [hash](const auto& pair) {
        return pair.first == hash;
    });
}
