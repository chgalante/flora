#include "Event.hpp"

namespace FloraEngine {
void Event::SetEventCallback(const std::function<void()> &event_callback) {
  mEventCallback = event_callback;
}
} // namespace FloraEngine