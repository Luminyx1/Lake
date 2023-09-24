#pragma once

#include "Lake/Common.h"

#include <glm/glm.hpp>

namespace lake {

    namespace EventType {
        enum {
            WindowResize,
            WindowMinimize,
            WindowMaximize,

            Count // Start your own events from here
        };
    } // namespace EventType

    class Event {
    public:
        virtual ~Event() = default;

        virtual u32 getType() const = 0;
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(i32 width, i32 height)
            : width_(width), height_(height)
        { }

        ~WindowResizeEvent() override = default;

        [[nodiscard]] glm::u32vec2 getSize() const { return glm::u32vec2(this->width_, this->height_); }

        u32 getType() const override { return EventType::WindowResize; }

    private:
        i32 width_;
        i32 height_;
    };

    class WindowMinimizeEvent : public Event {
    public:
        u32 getType() const override { return EventType::WindowMinimize; }
    };

    class WindowMaximizeEvent : public Event {
    public:
        u32 getType() const override { return EventType::WindowMaximize; }
    };

} // namespace lake
