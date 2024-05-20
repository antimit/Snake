#include "EventManager.h"

namespace GameEvent {

bool EventManager::skinChangeRequired = false;
std::string EventManager::lastEventData = ""; 

void EventManager::setSkinChangeRequired(bool value) {
    skinChangeRequired = value;
}

bool EventManager::isSkinChangeRequired() {
    return skinChangeRequired;
}

std::string EventManager::getLastEventData() {
    return lastEventData;
}

void EventManager::subscribe(const std::string& eventType, std::function<void(const Event&)> listener) {
    listeners[eventType].push_back(listener);
}

void EventManager::publish(const Event& event) {
    auto& subs = listeners[event.type];
    for (auto& listener : subs) {
        listener(event);
    }
    lastEventData = event.data; // Update last event data whenever an event is published
    skinChangeRequired = true; // Set the flag when a skin change event is published
}

}
