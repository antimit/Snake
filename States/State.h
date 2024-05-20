#ifndef STATE_H
#define STATE_H

#include "../Entity/Entity.h"


class State{
private:

protected:
    std::stack<State*>*states;
    sf::RenderWindow* window;
    std::map<std::string, int>* supportedKeys;
    std::map<std::string, int>keybinds;
    bool quit;
    bool paused;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Resources
    std::vector<sf::Texture> textures;

    //Functions

    virtual void initKeybinds() = 0;


public:
    State(sf::RenderWindow* window,std::map<std::string,int>* supportedKeys,std::stack<State*>*states);
    virtual ~State();

    const bool& getQuit() const;

    
    void endState();
    void pauseState();
    void unpauseState();
    void setupText(sf::Text *textItem, const sf::Font &font, const sf::String &value, int size, sf::Color colour);
    virtual void updateMousePositions();
    virtual void updateInput(const float & dt) = 0;


    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget* target = NULL) = 0;


};


#endif //STATE_H
