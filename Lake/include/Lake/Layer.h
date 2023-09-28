#pragma once

#include "Lake/Common.h"

#include "Lake/DrawableComponent.h"
#include "Lake/CameraComponent.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace lake {

    class Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer() = default;

        virtual void draw(const RenderInfo& renderInfo);
        virtual void resize(const glm::u32vec2& size);

        void setCamera(CameraComponent* camera) { mCamera = camera; }
        [[nodiscard]] CameraComponent* getCamera() const { return mCamera; }

    protected:
        friend class LayerStack;

        std::vector<DrawableComponent*> mDrawables;
        const std::string mName;
        CameraComponent* mCamera;
    };

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        template<typename T>
        T* pushLayer(const std::string& name) requires std::is_base_of_v<Layer, T> {
            T* layer = new T(name);
            mLayers.emplace_back(std::make_pair(std::hash<std::string>{}(name), layer));
            
            return layer;
        }

        void popLayer();
        void removeLayer(const std::string& name);
        void clearLayers();

        [[nodiscard]] Layer* getLayer(const std::size_t hash);
        [[nodiscard]] Layer* getLayer(const std::string& name) { return this->getLayer(std::hash<std::string>{}(name)); }

        template <typename T>
        T* getLayer(const std::string& name) {
            static_assert(std::is_base_of<Layer, T>::value, "T must derive from Layer");
        
            return static_cast<T*>(this->getLayer(name));
        }

        void resizeLayers(const glm::u32vec2& size);

        void pushDrawable(DrawableComponent* drawable, const std::size_t layerHash);

        void drawLayers() const;

    private:
        using LayerContainer = std::vector<std::pair<std::size_t, Layer*>>; // TODO: Use a map instead of a vector for faster lookup?

        LayerContainer::iterator getLayerIterator(const std::size_t hash);

        LayerContainer mLayers;
    };

} // namespace lake
