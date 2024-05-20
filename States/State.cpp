
#include "State.h"




State::State(sf::RenderWindow* window,std::map<std::string,int>* supportedKeys, std::stack<State*>*states) {

    this->supportedKeys = supportedKeys;
    this->window = window;
    this->states = states;
    this->quit= false;
    this->paused = false;
}

State::~State() {

}
const bool &State::getQuit() const {
    return this->quit;
}

void State::endState()
{
    this->quit = true;
}

void State::pauseState()
{
    this->paused = true;
}

void State::unpauseState()
{
    this->paused = false;
}

void State::setupText(sf::Text *textItem, const sf::Font &font, const sf::String &value, int size, sf::Color colour) {
  textItem->setFont(font);
  textItem->setString(value);
  textItem->setCharacterSize(size);
  textItem->setFillColor(colour);
}

void State::updateMousePositions() {
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}


