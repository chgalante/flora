
#pragma once
#include "Event.hpp"

namespace FloraEngine {

class KeyEvent : public Event {
public:
  KeyEvent() {}
  ~KeyEvent() {}

  SET_EVENT_CATEGORY(EventCategoryInput || EventCategoryKeyboard)
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(int key, int scancode, int mods)
      : mKey(key), mScanCode(scancode), mMods(mods) {}
  ~KeyPressedEvent() {}

  int GetKey() {
    return mKey;
  }
  int GetMods() {
    return mMods;
  }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << mMods << " + " << mKey << "(" << mScanCode
       << ")";
    return ss.str();
  }

  SET_EVENT_TYPE(KeyPressedEvent)
private:
  int mKey, mScanCode, mMods;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(int key, int scancode, int mods)
      : mKey(key), mScanCode(scancode), mMods(mods) {}
  ~KeyReleasedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << mMods << " + " << mKey << "(" << mScanCode
       << ")";
    return ss.str();
  }

  SET_EVENT_TYPE(KeyReleasedEvent)
private:
  int mKey, mScanCode, mMods;
};

class MouseButtonPressedEvent : public Event {
public:
  MouseButtonPressedEvent(int button, int mods)
      : mButton(button), mMods(mods) {}
  ~MouseButtonPressedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << mMods << " + " << mButton;
    return ss.str();
  }

  SET_EVENT_CATEGORY(EventCategoryInput || EventCategoryMouse ||
                     EventCategoryMouseButton)
  SET_EVENT_TYPE(MouseButtonPressedEvent)

private:
  int mButton, mMods;
};

class MouseButtonReleasedEvent : public Event {
public:
  MouseButtonReleasedEvent(int button, int mods)
      : mButton(button), mMods(mods) {}
  ~MouseButtonReleasedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << mMods << " + " << mButton;
    return ss.str();
  }

  SET_EVENT_CATEGORY(EventCategoryInput || EventCategoryMouse ||
                     EventCategoryMouseButton)
  SET_EVENT_TYPE(MouseButtonReleasedEvent)

private:
  int mButton, mMods;
};

class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(double xpos, double ypos) : mXpos(xpos), mYpos(ypos) {}
  ~MouseMovedEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << mXpos << ", " << mYpos;
    return ss.str();
  }

  SET_EVENT_CATEGORY(EventCategoryInput || EventCategoryMouse)
  SET_EVENT_TYPE(MouseMovedEvent)

private:
  double mXpos, mYpos;
};

} // namespace FloraEngine