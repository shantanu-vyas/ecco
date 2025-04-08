#ifndef ECCO_EVENT
#define ECCO_EVENT

#include <tuple>
#include <string>
#include <vector>

//when i accidentally included dispatcher in here the dispatcher file was unable to find eccoevent??

namespace ecco {
namespace Event {
//needs to inheir from eccoobject but also has a stirng constructor...
//fix this
class EccoEvent {
public:
    public:
    EccoEvent(std::string descriptor);
    virtual ~EccoEvent() = default;
    std::string GetDescriptor() const;

private:
  std::string m_descriptor;
};

class KeyEvent : public EccoEvent {
  public:
    //these should be like GLFW keys...
    using ModList = std::vector<int>;
    using KeyList = std::vector<int>;
  KeyEvent(std::string descriptor,
           const ModList &modIDs,
           const KeyList &keyIds);

    //it would be cool if i made it so only a publisher could modify an event to reuse it
    //but make it so a subscriber couldnt via constness or something
    const std::tuple<const ModList&, const KeyList&> GetData();


private:
  ModList m_modIDs;
  KeyList m_keyIDS;
};

// class UIClickEvent : public EccoEvent {
//   int clickType;
// };
//   class  ExitEvent : public EccoEvent {};

} // namespace Event
} // namespace ecco

#endif
