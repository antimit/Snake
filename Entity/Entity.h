#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "sstream"
#include <vector>
#include <stack>
#include <map>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "string"


class Entity {
private:
    void initVariables();

protected:
    sf::Texture* texture;
    sf::Sprite* sprite;
    
    float movementSpeed;

public:
    Entity();
    explicit Entity(sf::Vector2f position);
    virtual ~Entity();

    //Componet functions
    void createSprite(sf::Texture * texture);
    void setSpritePosition(const sf::Vector2f& position);
    sf::Sprite* getSprite() const;  // Return pointer for more complex operations
    void Load(std::string adress);
    

    //Functions

    virtual void move(const float& dt,const float x, const float y);

    virtual void update (const float&dt);
    virtual void render(sf::RenderTarget* target);
};


#endif
