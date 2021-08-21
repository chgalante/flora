#pragma once
#include "pch.h"

namespace FloraEngine {

enum EventCategory {
  Application,
  Keyboard,
  Mouse,
};

enum EventType {
  WindowClosed,
  WindowResized,
  KeyPressed,
  KeyReleased,
  MousePressed,
  MouseReleased,
  MouseMoved,
};

class Event {

protected:
  Event() {}
  ~Event() {}

  void SetEventCallback(const std::function<void()> &);

  virtual void Handle() = 0;

  std::function<void()> mEventCallback;
  bool                  mIsHandled;
};

class EventDispatcher {
public:
  EventDispatcher() {}
  ~EventDispatcher() {}
};
} // namespace FloraEngine