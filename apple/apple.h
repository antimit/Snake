#ifndef APPLE_HPP
#define APPLE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>



class Apple {
private:

    sf::RectangleShape sprite;
    sf::Texture texture;

public:
    Apple();
    

    void setPosition(sf::Vector2f newPosition);
    sf::RectangleShape getSprite();
};


#endif 