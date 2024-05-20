#include "MainMenu.h"



// Initializer functions 
void MainMenu::initVariables()
{
    resolution = sf::Vector2f(1920, 1080);
    

}

void MainMenu::initBackground()
{
    this->background.setSize(sf::Vector2f
    (
        static_cast<float>(this->window->getSize().x),
        static_cast<float>(this->window->getSize().x)
        )
    );

    

    if(!this->backgroundTexture.loadFromFile("../Resources/Images/Backgrounds/mainmenu.png"))
    {
        throw"ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
    }
    
    this->background.setTexture(&this->backgroundTexture);

    
}
void MainMenu::initFonts()
{
    if(!this->font.loadFromFile("../assets/Fonts/Dosis-Light.ttf"))
    {
        throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
    }
}

void MainMenu::initKeybinds()
{

    std::ifstream ifs("assets/Config/mainmenustate_keybinds.ini");
    if (ifs.is_open()){
        std::string key = "";
        std::string key2 = 0;

        while(ifs>>key>>key2)
        {
            this->keybinds[key] = this->supportedKeys->at(key2);
        }
    }
    ifs.close();

    this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");
   
}

void MainMenu::initButtons()
{
    this->buttons["newGame"] = new Button(200,300,400,100,&this->font,"StartTheGame",
                                             sf::Color(70,70,70,200),sf::Color(150,150,150,255),sf::Color(20,20,20,200));                               

    this->buttons["EXIT_STATE"] = new Button(200,600,150,50,&this->font,"QUIT",
                                             sf::Color(100,100,100,200),sf::Color(150,150,150,255),sf::Color(20,20,20,200));
}




MainMenu::MainMenu(sf::RenderWindow* window,std::map<std::string,int>* supportedKeys,std::stack<State*>*states )
        : State(window, supportedKeys,states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeybinds();
    this->initButtons();
    this->windowElements();
    

    
}

MainMenu::~MainMenu()
{
    auto it = this->buttons.begin();
    for(it = this->buttons.begin();it!= this->buttons.end();++it)
    {
        delete it->second;
    }
}




void MainMenu::setupText(Text *textItem, const Font &font, const String &value, int size, Color colour) {
  textItem->setFont(font);
  textItem->setString(value);
  textItem->setCharacterSize(size);
  textItem->setFillColor(colour);
}


void MainMenu::windowElements()
{
   mainFont.loadFromFile("../assets/fonts/slant_regular.ttf");
   sf::Color semiTransparentPurple(150, 50, 150, 128);
 

  setupText(&pressEnterText, mainFont, "WELCOME TO THE SNAKE GAME MAIN MENU", 38, semiTransparentPurple);
  FloatRect pressEnterTextBounds = pressEnterText.getLocalBounds();
  pressEnterText.setPosition(Vector2f(resolution.x/2 - pressEnterTextBounds.width / 2, 200));
  pressEnterText.setOutlineColor(Color::Black);
  pressEnterText.setOutlineThickness(2);
 

}


void MainMenu::updateButtons()
{
    /*Update all the buttons in the state and handles their functionality*/
    for (auto &it : this->buttons)
    {
        it.second->update(this->mousePosView);
    }

    //New game
    if(this->buttons["newGame"]->isPressed())
    {
        this->states->push(new GameState(this->window, this->supportedKeys,this->states));
    }

   

    //Quit the game
    if(this->buttons["EXIT_STATE"]->isPressed())
    {
        this->endState();
        
        
        
    }

}




void MainMenu::update(const float& dt) {
    this->updateMousePositions();
    this->updateInput(dt);
    this->updateButtons();



}

void MainMenu::updateInput(const float &dt) {
    // this->checkForQuit();
    

}
void MainMenu::renderButtons(sf::RenderTarget *target) {


    for (auto &it : this->buttons)
    {
        it.second->render(target);
    }

}

void MainMenu::render(sf::RenderTarget* target ) {
    if(!target)
        target = this->window;

    target->draw(this->background);

    target->draw(pressEnterText);

    this->renderButtons(target);
    
   
    sf::Text mouseText;
    mouseText.setPosition(this->mousePosView.x, this->mousePosView.y-50);
    mouseText.setFont(this->font);
    mouseText.setCharacterSize(12);
    std::stringstream ss;
    ss << this->mousePosView.x << " " << this->mousePosView.y;

    mouseText.setString(ss.str());

    target->draw(mouseText);

    

}