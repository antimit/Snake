#ifndef MainMenu_H
#define MainMenu_H

#include "GameState.h"
#include "../Resource Files/Button.h"


#include "State.h"

#include <SFML/System.hpp>



class MainMenu :
        public State
{
private:
    //Variables
    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;
    Text pressEnterText;

    sf::Font mainFont;
    sf::Vector2f resolution;
   
    std::map<std::string, Button*> buttons;

   

    //Functions
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initButtons();
    void windowElements();



public:
    MainMenu(sf::RenderWindow* window,std::map<std::string,int>* supportedKeys,std::stack<State*>*states);
    virtual ~MainMenu();

    //Functions

    static void setupText(Text *textItem, const Font &font, const String &value,
                        int size, Color colour);


    
    void updateButtons();
   void updateInput(const float &dt);
   
    void update(const float& dt);
    void renderButtons(sf::RenderTarget* target = NULL);
    void render(sf::RenderTarget* target = NULL);


};


#endif
