

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../SnakeSection/Snake.h"
#include "../SnakeSection/wall.hpp"
#include "../apple/apple.h"
#include "State.h"
#include <SFML/System.hpp>
#include <vector>
#include <unistd.h>
#include <limits.h>
#include <filesystem>
#include <random>
#include <vector>
#include "../Resource Files/Button.h"
#include <sqlite3.h>


class GameState : public State {
private:
  Entity player;
  Snake snake;
  Apple apple;
  const std::string DATABASE_PATH = "/home/antimit/Desktop/RPG1/current/States/highscore.db";

  sf::Vector2u windowSize;
  unsigned int windowWidth;
  unsigned int windowHeight;
  std::map<std::string, Button *> buttons;

  int applesEatenThisLevel;
  int applesEatenTotal;
  int score;
  bool border;

  sf::Font font;

  sf::Texture texture;
  sf::Sprite sprite;
  sf::Time timeSinceLastMove;
  sf::Time timeSinceLastMovePause;
  sf::Vector2f resolution;
  std::string newSkinPath;
  bool skinChangeRequired;
 std::default_random_engine randomEngine; 

  sf::Music music;

  // Functions
  void initKeybinds();
  int snakeDirection;
  std::deque<int> directionQueue; // queue for direction key presses
  int speed;
  int sectionsToAdd; // how many sections to add to the snake

  std::vector<Wall> wallSections;
  int currentLevel;
  int maxLevels;
  unsigned int applesForNextLevel = 2;
  std::vector<String> levels;
  bool Checked;

  Font mainFont;
  Text titleText;
  Text applesEatenText;
  Text currentLevelText;
  Text scoreText;
  Text gameOverText;
  Text pressEnterText;

  int currentGameState;
  int lastGameState; // For storing the last state the game was in when pausing.

public:
  enum gameState { RUNNING, PAUSED, GAMEOVER };
  enum Direction { UP, RIGHT, DOWN, LEFT };
  GameState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys,
            std::stack<State *> *states);
  virtual ~GameState();

  // Functions
  void updateSkin();
  void initDatabase();
  void saveScore();
  void changeSnakeSkin(const std::string &texturePath);

  void newSnake();
  std::string getDatabasePath();
  void moveApple();
  void windowElements();
  void movePlayer();
  void addDirection(int newDirection);
  void addSnakeSection();
  void checkLevelFiles();

  void startTheGame();
  void beginNextLevel();
  void loadLevel(int levelNumber);
 static void setupText(sf::Text* textItem, const sf::Font& font, const sf::String& value, int size, sf::Color color, float posX, float posY, bool centered = true);


  void renderButtons(sf::RenderTarget *target);

  void updateInput(const float &dt);
  void update(const float &dt);
  void render(sf::RenderTarget *target = NULL);
};

#endif // NIBBLES_01_GAMESTATE_H
