#pragma once
#include "Event.hpp"

namespace FloraEngine {

class WindowEvent : public Event {
public:
  WindowEvent() {}
  ~WindowEvent() {}

  SET_EVENT_CATEGORY(EventCategoryWindow)
};

class WindowClosedEvent : public WindowEvent {
public:
  WindowClosedEvent() {}
  ~WindowClosedEvent() {}

  SET_EVENT_TYPE(WindowClosedEvent)
};
class WindowResizedEvent : public WindowEvent {
public:
  WindowResizedEvent(int width, int height) : mWidth(width), mHeight(height) {}
  ~WindowResizedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizedEvent: " << mWidth << ", " << mHeight;
    return ss.str();
  }

  SET_EVENT_TYPE(WindowResizedEvent)

private:
  int mWidth, mHeight;
};
class WindowMovedEvent : public WindowEvent {
public:
  WindowMovedEvent(int xpos, int ypos) : mXpos(xpos), mYpos(ypos) {}
  ~WindowMovedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowMovedEvent: " << mXpos << ", " << mYpos;
    return ss.str();
  }

  SET_EVENT_TYPE(WindowMovedEvent)

private:
  int mXpos, mYpos;
};
class WindowLostFocusEvent : public WindowEvent {
public:
  WindowLostFocusEvent() {}
  ~WindowLostFocusEvent() {}

  SET_EVENT_TYPE(WindowLostFocusEvent)
};

} // namespace FloraEngine