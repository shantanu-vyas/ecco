#ifndef ECCO_EVENT
#define ECCO_EVENT

#include "ecco/EccoObject.hpp"
namespace ecco {
namespace Event {

struct EccoEvent {
  virtual ~EccoEvent() = default;
};
struct KeyEvent : EccoEvent {
  int keyID;
};
struct UIClickEvent : EccoEvent {
  int clickType;
};
struct ExitEvent : EccoEvent {};


} // namespace Event
} // namespace ecco

#endif
