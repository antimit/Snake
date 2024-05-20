#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace GameEvent {

struct Event {
    std::string type;
    std::string data;  
};

}

#endif
