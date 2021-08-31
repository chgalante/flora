#pragma once
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {

enum EventCategory {
  None                     = 0,
  EventCategoryWindow      = BIT(0),
  EventCategoryInput       = BIT(1),
  EventCategoryKeyboard    = BIT(2),
  EventCategoryMouse       = BIT(3),
  EventCategoryMouseButton = BIT(4),
};

enum class EventType {
  None = 0,
  WindowClosedEvent,
  WindowResizedEvent,
  WindowMovedEvent,
  WindowLostFocusEvent,
  KeyPressedEvent,
  KeyReleasedEvent,
  MouseButtonPressedEvent,
  MouseButtonReleasedEvent,
  MouseMovedEvent,
  MouseScrolledEvent,
};

#define SET_EVENT_CATEGORY(category)              \
  virtual int GetCategoryFlags() const override { \
    return category;                              \
  }

#define SET_EVENT_TYPE(type)                        \
  static EventType GetStaticType() {                \
    return EventType::type;                         \
  }                                                 \
  virtual EventType GetEventType() const override { \
    return GetStaticType();                         \
  }                                                 \
  virtual const char *GetName() const override {    \
    return #type;                                   \
  }

class Event {

public:
  friend class EventDispatcher;
  Event() {}
  ~Event() {}

  bool IsHandled();
  bool IsInCategory(const EventCategory &category);

  virtual EventType   GetEventType() const     = 0;
  virtual const char *GetName() const          = 0;
  virtual int         GetCategoryFlags() const = 0;
  virtual std::string ToString() const {
    return GetName();
  }

private:
  bool mIsHandled;
};

inline std::ostream &operator<<(std::ostream &os, const Event &e) {
  return os << e.ToString();
}

class EventDispatcher {
  using EventFn = std::function<bool()>;

public:
  EventDispatcher(Event &e) : mEvent(&e){};

  inline void Dispatch(EventType type, EventFn callback) {
    if (mEvent->IsHandled())
      return;

    if (type == mEvent->GetEventType()) {
      mEvent->mIsHandled = callback();
    }
  }

private:
  Event *mEvent;
};

} // namespace FloraEngine