#pragma once

#include "Lake/Common.h"

#include "Lake/Drawable.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace lake {

    class Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer() = default;

        virtual void draw() = 0;
        virtual void resize(const glm::u32vec2& size) { }

    protected:
        friend class LayerStack;

        std::vector<Drawable*> mDrawables;
        const std::string mName;
    };

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        template<typename T>
        T* pushLayer(const std::string& name) {
            static_assert(std::is_base_of<Layer, T>::value, "T must derive from Layer");

            T* layer = new T(name);
            mLayers.emplace_back(std::make_pair(std::hash<std::string>{}(name), layer));
            return layer;
        }

        void popLayer();
        void removeLayer(const std::string& name);
        void clearLayers();

        [[nodiscard]] Layer* getLayer(const std::string& name);

        template <typename T>
        T* getLayer(const std::string& name) {
            static_assert(std::is_base_of<Layer, T>::value, "T must derive from Layer");
        
            return static_cast<T*>(this->getLayer(name));
        }

        void resizeLayers(const glm::u32vec2& size);

        void pushDrawable(Drawable* drawable, const std::size_t layerHash);

        void drawLayers() const;

    private:
        using LayerContainer = std::vector<std::pair<std::size_t, Layer*>>;

        LayerContainer::iterator getLayerIterator(const std::size_t hash);

        LayerContainer mLayers;
    };

} // namespace lake
