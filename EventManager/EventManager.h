#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <functional>

namespace GameEvent {

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }

    void subscribe(const std::string& eventType, std::function<void(const Event&)> listener);
    void publish(const Event& event);
    
    static void setSkinChangeRequired(bool value);
    static bool isSkinChangeRequired();

    static std::string getLastEventData(); // Getter for the last event data

private:
    EventManager() {}
    std::map<std::string, std::vector<std::function<void(const Event&)>>> listeners;

    static bool skinChangeRequired;
    static std::string lastEventData; // Store the last event data
};

}

#endif
