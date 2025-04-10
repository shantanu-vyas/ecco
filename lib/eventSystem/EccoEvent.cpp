#include "EccoEvent.hpp"
#include "ecco/EccoObject.hpp"
#include <typeindex>

using namespace ecco::Event;

template<typename EventType>
std::string TypeLookup() {
  return EventType::GetDescriptor();
}


EccoEvent::EccoEvent(std::string name)
    : ecco::EccoObject(std::move(name)) {}

EccoEvent::EccoEvent(const EccoEvent& event) :
  EccoObject(event.GetName()),
  m_descriptor(event.m_descriptor) {}


KeyEvent::KeyEvent(std::string name, const ModList &modIDs,
                   const KeyList &keyIds)
    : EccoEvent(name), m_modIDs(modIDs), m_keyIDS(keyIds) {}

const std::tuple<const KeyEvent::ModList&, const KeyEvent::KeyList&> KeyEvent::GetData() {
  return std::make_tuple<const KeyEvent::ModList&, const KeyEvent::KeyList&>(m_modIDs, m_keyIDS);
}
