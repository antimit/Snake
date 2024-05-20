#include "../Entity/Entity.h"
#include <SFML/Graphics.hpp>

class SnakeSection
:public Entity
{
public:
    
    // sf::Sprite  sprite;
    sf::Vector2f direction;  
    sf::Vector2f position;  
    // std::string adress1 = "../assets/Player/Textures/grass1.png";
    std::string adress1;
    // sf::RectangleShape shape;
    // sf::Sprite temp;
    
public:
    SnakeSection(sf::Vector2f startPosition);
    // virtual void update(const float &dt) override;

    void initVariables();
    
   
    void update();
    virtual void render(sf::RenderTarget* target) override;
    
    sf::Vector2f getPosition();
    
    void setPosition(sf::Vector2f newPosition);
    
    sf::Sprite getShape();






};