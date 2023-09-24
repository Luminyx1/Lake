#pragma once

#include "Lake/Common.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lake {

    namespace EventType {
        enum {
            WindowResize,
            WindowMinimize,
            WindowMaximize,

            KeyPress,
            KeyRelease,
            KeyRepeat,

            MousePress,
            MouseRelease,
            MouseMove,
            MouseScroll,

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
            : mWidth(width), mHeight(height)
        { }

        ~WindowResizeEvent() override = default;

        [[nodiscard]] glm::u32vec2 getSize() const { return glm::u32vec2(mWidth, mHeight); }

        u32 getType() const override { return EventType::WindowResize; }

    private:
        i32 mWidth;
        i32 mHeight;
    };

    class WindowMinimizeEvent : public Event {
    public:
        u32 getType() const override { return EventType::WindowMinimize; }
    };

    class WindowMaximizeEvent : public Event {
    public:
        u32 getType() const override { return EventType::WindowMaximize; }
    };

    class KeyEvent : public Event {
    public:
        KeyEvent(i32 key, i32 scancode, i32 action, i32 mods)
            : mKey(key), mScanCode(scancode), mAction(action), mMods(mods)
        { }

        ~KeyEvent() override = default;

        [[nodiscard]] i32 getKey() const { return mKey; }
        [[nodiscard]] i32 getScancode() const { return mScanCode; }
        [[nodiscard]] i32 getAction() const { return mAction; }
        [[nodiscard]] i32 getMods() const { return mMods; }

    private:
        i32 mKey;
        i32 mScanCode;
        i32 mAction;
        i32 mMods;
    };

    class KeyPressEvent : public KeyEvent {
    public:
        KeyPressEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(key, scancode, GLFW_PRESS, mods)
        { }

        u32 getType() const override { return EventType::KeyPress; }
    };

    class KeyReleaseEvent : public KeyEvent {
    public:
        KeyReleaseEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(key, scancode, GLFW_RELEASE, mods)
        { }

        u32 getType() const override { return EventType::KeyRelease; }
    };

    class KeyRepeatEvent : public KeyEvent {
    public:
        KeyRepeatEvent(i32 key, i32 scancode, i32 mods)
            : KeyEvent(key, scancode, GLFW_REPEAT, mods)
        { }

        u32 getType() const override { return EventType::KeyRepeat; }
    };

    class MouseEvent : public Event {
    public:
        MouseEvent(i32 button, i32 action, i32 mods)
            : mButton(button), mAction(action), mMods(mods)
        { }

        ~MouseEvent() override = default;

        [[nodiscard]] i32 getButton() const { return mButton; }
        [[nodiscard]] i32 getAction() const { return mAction; }
        [[nodiscard]] i32 getMods() const { return mMods; }

    private:
        i32 mButton;
        i32 mAction;
        i32 mMods;
    };

    class MousePressEvent : public MouseEvent {
    public:
        MousePressEvent(i32 button, i32 mods)
            : MouseEvent(button, GLFW_PRESS, mods)
        { }

        u32 getType() const override { return EventType::MousePress; }
    };

    class MouseReleaseEvent : public MouseEvent {
    public:
        MouseReleaseEvent(i32 button, i32 mods)
            : MouseEvent(button, GLFW_RELEASE, mods)
        { }

        u32 getType() const override { return EventType::MouseRelease; }
    };

    class MouseMoveEvent : public Event {
    public:
        MouseMoveEvent(f64 x, f64 y)
            : mX(x), mY(y)
        { }

        ~MouseMoveEvent() override = default;

        [[nodiscard]] glm::vec2 getPosition() const { return glm::vec2(mX, mY); }

        u32 getType() const override { return EventType::MouseMove; }

    private:
        f64 mX;
        f64 mY;
    };

    class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent(f64 x, f64 y)
            : mX(x), mY(y)
        { }

        ~MouseScrollEvent() override = default;

        [[nodiscard]] glm::vec2 getOffset() const { return glm::vec2(mX, mY); }

        u32 getType() const override { return EventType::MouseScroll; }

    private:
        f64 mX;
        f64 mY;
    };

} // namespace lake
