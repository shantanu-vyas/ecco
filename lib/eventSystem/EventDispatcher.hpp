#ifndef EVENT_DISPATCHER
#define EVENT_DISPATCHER

#include "ecco/EccoObject.hpp"
#include "eventSystem/EccoEvent.hpp"
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

            auto wrapper = [handler](const EccoEvent& event) {
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


        void dispatch(const EccoEvent& event) const {
            auto type = std::type_index(typeid(event));
            if (m_handlers.count(type)) {
                for (auto& handler : m_handlers.at(type)) {
                    handler(event);
                }
            }
        };

        private:
        //My handler functions for events
        std::unordered_map<std::type_index, std::function<void(const EccoEvent&)>> m_singleHandlers;
        //Other dispatchers that are listening for my events
        std::unordered_map<std::type_index, std::vector<std::function<void(const EccoEvent&)>>> m_handlers;
};
} // namespace Event
} // namespace ecco

#endif
