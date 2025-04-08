#include "EccoEvent.hpp"

using namespace ecco::Event;

EccoEvent::EccoEvent(std::string descriptor)
    : m_descriptor(std::move(descriptor)) {}

std::string EccoEvent::GetDescriptor() const { return m_descriptor; }

//////
KeyEvent::KeyEvent(std::string descriptor, const ModList &modIDs,
                   const KeyList &keyIds)
    : EccoEvent(descriptor), m_modIDs(modIDs), m_keyIDS(keyIds) {}

const std::tuple<const ModList&, const KeyList&> KeyEvent::GetData() {
      return std::make_tuple<const ModList&, const KeyEvent&>(m_modIDs, m_keyIDS);
}
