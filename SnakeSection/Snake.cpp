#include "Snake.h"

Snake::Snake() {
  int constanta = 20;
  int initialSectionCount = 200;
  
  snake.reserve(initialSectionCount); 
  
  snake.clear();
  snake.emplace_back(sf::Vector2f(100,100));
  
  snake.emplace_back(sf::Vector2f(100,100));
  snake.emplace_back(sf::Vector2f(100,100));

  speed = 2;
  snakeDirection = Direction::RIGHT;

  

 
}



void Snake::render(sf::RenderTarget *target) {
  // Render each section of the snake
  for (auto &section : snake) {
    section.render(target);
  }
  
  
}