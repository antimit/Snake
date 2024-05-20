#include "GameState.h"
#include "../GameStates/PauseState.h"
#include "../headers/Game.h"
#include "MainMenu.h"

#include <fstream>
#include <string>
#include <unistd.h>

void GameState::initKeybinds() {

  this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");
  this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
  this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
  this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
  this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");
  this->keybinds["PAUSE"] = this->supportedKeys->at("P");
}
GameState::GameState(sf::RenderWindow *window,
                     std::map<std::string, int> *supportedKeys,
                     std::stack<State *> *states)
    : State(window, supportedKeys, states) {

  this->initKeybinds();

  this->initDatabase();
  speed = 2;
  snakeDirection = Direction::RIGHT;
  timeSinceLastMove = sf::Time::Zero;
  Checked = false;

  sectionsToAdd = 0;
  std::string adress1 = "../assets/Player/Textures/grass1.png";

  maxLevels = 0;

  resolution = sf::Vector2f(1920, 1080);
  checkLevelFiles();
  std::cout << maxLevels << std::endl;

  windowElements();
  startTheGame();

  sf::Vector2u windowSize = window->getSize();
  windowWidth = windowSize.x;
  windowHeight = windowSize.y;

  currentGameState = GameState::RUNNING;
  lastGameState = currentGameState;
  std::random_device rd;  // Create a random device
  randomEngine.seed(rd()); 
}

GameState::~GameState() {}

void GameState::update(const float &dt) {

  this->updateMousePositions();
  this->updateInput(dt);
  this->updateSkin();
}

void GameState::addDirection(int newDirection) {
  if (directionQueue.empty()) {
    directionQueue.emplace_back(newDirection);
  } else {
    if (directionQueue.back() != newDirection) {
      directionQueue.emplace_back(newDirection);
    }
  }
}

void GameState::newSnake() {
  
  snake.snake.clear();
  snake.snake.emplace_back(Vector2f(100, 100));
  snake.snake.emplace_back(Vector2f(80, 100));
  snake.snake.emplace_back(Vector2f(60, 100));
  snake.snake[0].Load("../Resources/Images/Icons/head.png");
}

void GameState::startTheGame() {
  score = 0;
  speed = 2;
  snakeDirection = Direction::RIGHT;
  sectionsToAdd = 0;
  timeSinceLastMove = Time::Zero;
  wallSections.clear();
  directionQueue.clear();
  applesEatenThisLevel = 0;
  applesEatenTotal = 0;
  currentLevel = 1;
  loadLevel(currentLevel);
  newSnake();
  moveApple();
  this->border = false;

  currentGameState = GameState::RUNNING;
  lastGameState = currentGameState;
  currentLevelText.setString("level " + std::to_string(currentLevel));
  applesEatenText.setString("apples " + std::to_string(applesEatenTotal));
  FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
  applesEatenText.setPosition(Vector2f(
      currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));
  scoreText.setString(std::to_string(score));
  FloatRect scoreTextBounds = scoreText.getLocalBounds();
  scoreText.setPosition(
      Vector2f(resolution.x - scoreTextBounds.width - 15, -9));

  if (!music.openFromFile("../Resources/Music/music.wav")) {
    std::cout << "Doesn't work" << std::endl;
  }
  music.setVolume(100);
  music.play();

  snake.snake[0].Load("../Resources/Images/Icons/head.png");
}


void GameState::initDatabase() {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc = sqlite3_open(DATABASE_PATH.c_str(), &db); 

    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS HighScores ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Score INTEGER NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

void GameState::saveScore() {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc = sqlite3_open(DATABASE_PATH.c_str(), &db);

    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::string sqlInsert = "INSERT INTO HighScores (Score) VALUES (" + std::to_string(score) + ");";

    rc = sqlite3_exec(db, sqlInsert.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}












void GameState::windowElements() {
   
    if (!mainFont.loadFromFile("../assets/fonts/slant_regular.ttf")) {
        std::cerr << "Failed to load font from ../assets/fonts/slant_regular.ttf" << std::endl;
        return;  
    }

 
    setupText(&titleText, mainFont, "Snake", 28, Color::Blue, resolution.x / 2, -9, true);
    setupText(&gameOverText, mainFont, "GAME OVER", 72, Color::Yellow, resolution.x / 2, 100, true);
    gameOverText.setOutlineColor(Color::Black);
    gameOverText.setOutlineThickness(2);
    setupText(&pressEnterText, mainFont, "Enter - try again", 38, Color::Green, resolution.x / 2, 200, true);
    pressEnterText.setOutlineColor(Color::Black);
    pressEnterText.setOutlineThickness(2);

    
    setupText(&currentLevelText, mainFont, "level 1", 28, Color::Magenta, 15, -9, false);

   
    FloatRect currentLevelTextBounds = currentLevelText.getLocalBounds();  
    setupText(&applesEatenText, mainFont, "apples 0", 28, Color::Magenta, currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9, false);
}


void GameState::renderButtons(sf::RenderTarget *target) {

  for (auto &it : this->buttons) {
    it.second->render(target);
  }
}

void GameState::setupText(sf::Text* textItem, const sf::Font& font, const sf::String& value, int size, sf::Color color, float posX, float posY, bool centered) {
    textItem->setFont(font);
    textItem->setString(value);
    textItem->setCharacterSize(size);
    textItem->setFillColor(color);
    FloatRect textBounds = textItem->getLocalBounds();
    if (centered) {
        textItem->setPosition(Vector2f(posX - textBounds.width / 2, posY));
    } else {
        textItem->setPosition(Vector2f(posX, posY));
    }
}



void GameState::beginNextLevel() {
  
  currentLevel += 1;
  wallSections.clear();
  directionQueue.clear();
  speed = 2 + currentLevel;
  snakeDirection = Direction::RIGHT;
  sectionsToAdd = 0;
  applesEatenThisLevel = 0;

  loadLevel(currentLevel);
  newSnake();
  moveApple();
  currentLevelText.setString("level " + std::to_string(currentLevel));
  FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
  applesEatenText.setPosition(Vector2f(
      currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));
}

void GameState::checkLevelFiles() {
  // Load the levels manifest file
  std::ifstream levelsManifest("../assets/levels/levels.txt");
  std::ifstream testFile;
  for (std::string manifestLine; getline(levelsManifest, manifestLine);) {
    // Check that we can open the level file
    testFile.open("../assets/levels/" + manifestLine);
    if (testFile.is_open()) {
      // The level file opens, lets add it to the list of available levels
      levels.emplace_back("../assets/levels/" + manifestLine);
      testFile.close();

      maxLevels++;
    }
  }
}

void GameState::loadLevel(int levelNumber) {
  std::string levelFile = levels[levelNumber - 1];
  std::ifstream level(levelFile);
  std::string line;
  if (level.is_open()) {
    for (int y = 0; y < 54; y++) {
      getline(level, line);
      for (int x = 0; x < 96; x++) {
        if (line[x] == 'x') {
          wallSections.emplace_back(
              Wall(Vector2f(x * 20, y * 20), Vector2f(20, 20)));
        }
      }
    }
  }
  level.close();
}

void GameState::changeSnakeSkin(const std::string &texturePath) {
  for (auto &section : snake.snake) {
    section.Load(texturePath);
  }
  snake.snake[0].Load("../Resources/Images/Icons/head.png");
}

void GameState::moveApple() {
    const int gridUnit = 20; // Size of one grid unit
    sf::Vector2u windowSize = window->getSize();

    // Calculate the number of grid units in each dimension, subtracting the exterior walls
    int gridWidth = (windowSize.x / gridUnit) - 2;
    int gridHeight = (windowSize.y / gridUnit) - 2;

    std::uniform_int_distribution<int> distX(1, gridWidth);
    std::uniform_int_distribution<int> distY(1, gridHeight);

    sf::Vector2f newAppleLocation;
    bool badLocation;

    do {
        badLocation = false;
        newAppleLocation.x = distX(randomEngine) * gridUnit;
        newAppleLocation.y = distY(randomEngine) * gridUnit;

        // Check if the new location is inside the snake or on a wall
        for (auto &section : snake.snake) {
            if (section.getShape().getGlobalBounds().intersects(
                    sf::Rect<float>(newAppleLocation.x, newAppleLocation.y, gridUnit, gridUnit))) {
                badLocation = true;
                break;
            }
        }

    } while (badLocation);

    apple.setPosition(newAppleLocation);
}

void GameState::updateInput(const float &dt) {

  // Update player input
  if (sf::Keyboard::isKeyPressed(
          sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT")))) {

    addDirection(Direction::LEFT);
  }

  if (sf::Keyboard::isKeyPressed(
          sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT")))) {

    addDirection(Direction::RIGHT);
  }

  if (sf::Keyboard::isKeyPressed(
          sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))) {

    addDirection(Direction::UP);
  }

  if (sf::Keyboard::isKeyPressed(
          sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {

    addDirection(Direction::DOWN);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Enter) &&
      currentGameState == gameState::GAMEOVER) {
    startTheGame();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
    // this->endState();
    this->states->push(new PauseState(this->window, this->supportedKeys,
                                      this->states, music, border));

  this->timeSinceLastMove += sf::seconds(dt);
  if (this->timeSinceLastMove >= sf::seconds(1.f / float(this->speed))) {

    if (!(currentGameState == GameState::PAUSED ||
          currentGameState == GameState::GAMEOVER)) {
      this->movePlayer();
    }

    this->timeSinceLastMove = sf::Time::Zero;
  }
}

void GameState::updateSkin() {

  if (GameEvent::EventManager::isSkinChangeRequired()) {

    std::string newSkinPath = GameEvent::EventManager::getLastEventData();
    changeSnakeSkin(newSkinPath);
    GameEvent::EventManager::setSkinChangeRequired(false);
  }
}

void GameState::movePlayer() {
  sf::Vector2f thisSectionPosition = snake.snake[0].getPosition();
  sf::Vector2f lastSectionPosition = thisSectionPosition;

  if (!directionQueue.empty()) {
    // Make sure we don't reverse direction
    switch (snakeDirection) {
    case Direction::UP:
      if (directionQueue.front() != Direction::DOWN) {
        snakeDirection = directionQueue.front();
      }
      break;
    case Direction::DOWN:
      if (directionQueue.front() != Direction::UP) {
        snakeDirection = directionQueue.front();
      }
      break;
    case Direction::LEFT:
      if (directionQueue.front() != Direction::RIGHT) {
        snakeDirection = directionQueue.front();
      }
      break;
    case Direction::RIGHT:
      if (directionQueue.front() != Direction::LEFT) {
        snakeDirection = directionQueue.front();
      }
      break;
    }
    directionQueue.pop_front();
  }

 
  if (sectionsToAdd) {
    addSnakeSection();
    sectionsToAdd--;
  }

  // Update the snake's head position
  switch (snakeDirection) {
  case Direction::RIGHT:
    snake.snake[0].setPosition(
        sf::Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
    break;
  case Direction::DOWN:
    snake.snake[0].setPosition(
        sf::Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
    break;
  case Direction::LEFT:
    snake.snake[0].setPosition(
        sf::Vector2f(thisSectionPosition.x - 20, thisSectionPosition.y));
    break;
  case Direction::UP:
    snake.snake[0].setPosition(
        sf::Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
    break;
  }

  if (border == false) {

    // Handle wrap-around at screen edges
    if (snake.snake[0].getPosition().x < 0) {
      snake.snake[0].setPosition(
          sf::Vector2f(static_cast<float>(window->getSize().x),
                       snake.snake[0].getPosition().y));
    } else if (snake.snake[0].getPosition().x >= window->getSize().x) {
      snake.snake[0].setPosition(
          sf::Vector2f(0, snake.snake[0].getPosition().y));
    }
    if (snake.snake[0].getPosition().y < 0) {
      snake.snake[0].setPosition(
          sf::Vector2f(snake.snake[0].getPosition().x,
                       static_cast<float>(window->getSize().y)));
    } else if (snake.snake[0].getPosition().y >= window->getSize().y) {
      snake.snake[0].setPosition(
          sf::Vector2f(snake.snake[0].getPosition().x, 0));
    }
  }

  // Update the snake tail positions
  for (int s = 1; s < snake.snake.size(); s++) {
    thisSectionPosition = snake.snake[s].getPosition();
    snake.snake[s].setPosition(lastSectionPosition);
    lastSectionPosition = thisSectionPosition;
  }

  // Update snake sections
  for (auto &s : snake.snake) {
    s.update();
  }

  // Collision detection - Apple
  if (snake.snake[0].getShape().getGlobalBounds().intersects(
          apple.getSprite().getGlobalBounds())) {
    applesEatenThisLevel += 1;
    applesEatenTotal += 1;
    applesEatenText.setString("apples " + std::to_string(applesEatenTotal));
    sf::FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
    applesEatenText.setPosition(sf::Vector2f(
        currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));

    bool beginningNewLevel = false;
    if (applesEatenThisLevel >= applesForNextLevel) {
      if (currentLevel < maxLevels) {
        applesForNextLevel += 2;
        beginningNewLevel = true;
        beginNextLevel();
      }
    }

    if (!beginningNewLevel) {
      sectionsToAdd += 4;
      speed++;
      moveApple();
    }
  }

  // Collision detection - Snake Body
  for (int s = 1; s < snake.snake.size(); s++) {
    if (snake.snake[0].getShape().getGlobalBounds().intersects(
            snake.snake[s].getShape().getGlobalBounds())) {
      currentGameState = GameState::GAMEOVER;
      window->draw(gameOverText);
      window->draw(pressEnterText);
      return;
    }
  }

  // Collision detection - Wall
  if (border) {
    for (auto &w : wallSections) {
      if (snake.snake[0].getShape().getGlobalBounds().intersects(
              w.getShape().getGlobalBounds())) {
        currentGameState = GameState::GAMEOVER;
        window->draw(gameOverText);
        window->draw(pressEnterText);
        return;
      }
    }
  }
}

void GameState::addSnakeSection() {

  sf::Vector2f newSectionPosition =
      snake.snake[snake.snake.size() - 1].getPosition();
  snake.snake.emplace_back(newSectionPosition);
  snake.snake[snake.snake.size() - 1].Load(
      GameEvent::EventManager::getLastEventData());
}

void GameState::render(sf::RenderTarget *target) {

  if (!target)
    target = this->window;

  if (border) {

    for (auto &w : wallSections) {
      target->draw(w.getShape());
    }
  }

  target->draw(titleText);
  target->draw(currentLevelText);
  target->draw(applesEatenText);
  target->draw(scoreText);

  this->snake.render(target);

  target->draw(apple.getSprite());

  // Draw GameOver

  if (currentGameState == GameState::GAMEOVER) {

    target->draw(gameOverText);
    target->draw(pressEnterText);
  }

  if (currentGameState == GameState::GAMEOVER && Checked == false) {
    score = applesEatenTotal;
    saveScore();

    Checked = true;
  }
}
