#pragma once
#include "ApplicationEvent.hpp"
#include "Event.hpp"
#include "Flora/Utilities/Log.hpp"

namespace FloraEngine {

class ApplicationEvent : public Event {
public:
  ApplicationEvent() {}
  ~ApplicationEvent() {}

  FloraEngine::EventCategory GetCategory() {
    return FloraEngine::EventCategory::Application;
  }
};

class WindowClosedEvent : public ApplicationEvent {
public:
  WindowClosedEvent() {}
  ~WindowClosedEvent() {}

  FloraEngine::EventType GetType() {
    return FloraEngine::EventType::WindowClosed;
  }

  inline void Handle() override {
    FE_CORE_TRACE("WindowClosedEvent");
  }
};
} // namespace FloraEngine