#include "EccoEvent.hpp"

using namespace ecco::Event;

EccoEvent::EccoEvent(std::string descriptor)
    : m_descriptor(std::move(descriptor)) {}

std::string EccoEvent::GetDescriptor() const { return m_descriptor; }

//////
KeyEvent::KeyEvent(std::string descriptor, const ModList &modIDs,
                   const KeyList &keyIds)
    : EccoEvent(descriptor), m_modIDs(modIDs), m_keyIDS(keyIds) {}

const std::tuple<const KeyEvent::ModList&, const KeyEvent::KeyList&> KeyEvent::GetData() {
  return std::make_tuple<const KeyEvent::ModList&, const KeyEvent::KeyList&>(m_modIDs, m_keyIDS);
}
