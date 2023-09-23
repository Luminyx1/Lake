#include "Lake/Layer.h"

#include "Lake/Log.h"

lake::Layer::Layer(const std::string& name)
    : mDrawables()
    , mName(name)
{ }

lake::LayerStack::LayerStack()
    : mLayers()
{ }

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

void lake::LayerStack::resizeLayers(const glm::u32vec2& size) {
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
    for (const auto& [hash, layer] : mLayers) {
        layer->draw();
        layer->mDrawables.clear();
    }
}

lake::LayerStack::LayerContainer::iterator lake::LayerStack::getLayerIterator(const std::size_t hash) {
    return std::find_if(mLayers.begin(), mLayers.end(), [hash](const auto& pair) {
        return pair.first == hash;
    });
}
