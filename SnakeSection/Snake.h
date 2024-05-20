#include "snakesection.h"
#include <deque>
#include <vector>
class Snake {
private:
  int snakeDirection;
  std::deque<int> directionQueue; // queue for direction key presses
  int speed;
  int sectionsToAdd; // how many sections to add to the snake

  

public:
  std::vector<SnakeSection> snake;
  enum Direction { UP, RIGHT, DOWN, LEFT };
  Snake();
 

  void render(sf::RenderTarget *target);
};