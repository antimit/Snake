#include "snakesection.h"


void SnakeSection::initVariables()
{
    this->adress1 = "../Resources/Images/Icons/body.png";
}

SnakeSection::SnakeSection(sf::Vector2f startPosition) : position(startPosition) {
    initVariables();
    Entity::Load(adress1);
    if (Entity::sprite) {
        Entity::sprite->setPosition(position);
    }
    
}






void SnakeSection::render(sf::RenderTarget *target) {
  // Render: The snake section might have a special way of rendering.
  Entity::render(target); // Call the base class render function.
//   std::cout << "Rendering at position: " << sprite.getPosition().x << ", "
//             << sprite.getPosition().y << std::endl;
}



sf::Vector2f SnakeSection::getPosition() { return position; }

void SnakeSection::setPosition(sf::Vector2f newPosition) {
  position = newPosition;
}
void SnakeSection::update()
{
  this->sprite->setPosition(position);
}

// void SnakeSection::update() { sprite.setPosition(position); }



sf::Sprite SnakeSection::getShape()
{
  return *sprite;
}

