#include "Event.hpp"

namespace FloraEngine {

bool Event::IsInCategory(const EventCategory &category) {
  return GetCategoryFlags() & category;
}

bool Event::IsHandled() {
  return mIsHandled;
}
} // namespace FloraEngine