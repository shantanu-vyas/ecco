#ifndef EVENT_DISPATCHER
#define EVENT_DISPATCHER

#include "ecco/EccoObject.hpp"
#include "eventSystem/EccoEvent.hpp"
#include <sstream>
#include <typeindex>
#include <unordered_map>

namespace ecco {
namespace Event {

class EventDispatcher : public ecco::EccoObject {
    public:

        template<typename EventType>
        using HandlerFunc = std::function<void(const EventType)>;

        EventDispatcher(std::string name) : EccoObject(name) {};
        ~EventDispatcher() = default;

        template<typename EventType>
        void SetEventCallback(HandlerFunc<EventType> handler) {
            auto type = std::type_index(typeid(EventType));

            auto wrapper = [handler](const ecco::Event::EccoEvent& event) {
                handler(static_cast<const EventType&>(event));
            };
            m_singleHandlers[type] = std::move(wrapper);
        }

        template<typename EventType>
        std::function<void(const EccoEvent&)> GetTypeHandler() const {
            auto type = std::type_index(typeid(EventType));
            return m_singleHandlers.at(type);
        }

        template<typename EventType>
        void AddListener(const EventDispatcher& ed) {
            auto type = std::type_index(typeid(EventType));
            m_handlers[type].emplace_back(ed.GetTypeHandler<EventType>());
        }


        void dispatch(const ecco::Event::EccoEvent& event) const {
            //maybe change handler func so it also takes in a const ref
            //to a EventDispacher thats dispatching so the handle func can
            //print the source of the event for debugging?
            auto type = std::type_index(typeid(event));
            if (m_handlers.count(type)) {
                for (auto& handler : m_handlers.at(type)) {
                    handler(event);
                }
            }
        };

        //add listener would give us the data
        //maybe event could have a source and destination weakptr to an event dispatcher?
        std::string PrintListenInfo() const {
            //NYI -> it would be cool to print all known sources/origins of the event
        }
        std::string PrintDispatchInfo() const {
            //NYI -> print all known destinations?
        }
        std::string PrintInfo() const {
            //NYI
        }

        private:
        //My handler functions for events
        std::unordered_map<std::type_index, std::function<void(const EccoEvent&)>> m_singleHandlers;
        //Other dispatchers that are listening for my events
        std::unordered_map<std::type_index, std::vector<std::function<void(const EccoEvent&)>>> m_handlers;
};
} // namespace Event
} // namespace ecco

#endif
