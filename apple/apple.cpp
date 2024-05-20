#include "apple.h"

Apple::Apple() {
  if (!texture.loadFromFile("../Resources/Images/Icons/appl.png")) {  
            // Handle error
            std::cerr << "Failed to load texture!" << std::endl;
        }

        sf::Vector2f startingPosition(400, 300);
        sprite.setSize(sf::Vector2f(20, 20));
        sprite.setTexture(&texture);  // Set the texture to the sprite
        sprite.setPosition(startingPosition);
  
}

void Apple::setPosition(sf::Vector2f newPosition) {
  sprite.setPosition(newPosition);
}

sf::RectangleShape Apple::getSprite() {
  return sprite;
}