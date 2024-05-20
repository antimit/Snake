#ifndef PauseState_H
#define PauseState_H
#include "../States/GameState.h"
#include "../Resource Files/Button.h"


#include "../States/State.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <sqlite3.h>
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"
#include "../States/GameState.h"
#include <unistd.h>
#include <limits.h>
#include <filesystem>





class PauseState :
        public State
{
private:
    //Variables
    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;
    Text pressEnterText;
    Text volumeText;

    sf::Font mainFont;
    sf::Vector2f resolution;
    sf::Texture * texture_Scroll;
    const std::string DATABASE_PATH = "/home/antimit/Desktop/RPG1/current/States/highscore.db";
    sf::Sprite * sprite_Scroll;
    std::map<std::string, Button*> buttons;


    sf::RectangleShape track;
    sf::Music music;
    sf::Music * mus;
    sf::Texture* texture1;
    sf::Texture* texture2;
    

    sf::RectangleShape slider;

    bool dragging;
    float volume;

    bool *Border;
    bool results;
   

    
    // sf::Vector2i mousePosView;

    //Functions
    void initSlider();
    void setSkin();
    std::vector<std::pair<int, int>> fetchHighScores();
    void initVariables();
    void initBackground();
    void initFonts();
    
    void initKeybinds();
    void initButtons();
    void windowElements();
    



public:
    PauseState(sf::RenderWindow* window,std::map<std::string,int>* supportedKeys,std::stack<State*>*states,sf::Music &musiC,bool &border);
    virtual ~PauseState();

    //Functions

    static void setupText(Text *textItem, const Font &font, const String &value,
                        int size, Color colour);


    void updateInput(const float& dt);
    void updateButtons();
  

    void update(const float& dt);
    void renderButtons(sf::RenderTarget* target = NULL);
    void render(sf::RenderTarget* target = NULL);


};


#endif
