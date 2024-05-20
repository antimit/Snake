#include "Game.h"
#include <filesystem>




// Initializer
void Game::initVariables() {
  this->window = NULL;

  this->fullscreen = false;
  this->dt = 0.f;
}

void Game::initWindow() {

  this->videoModes = sf::VideoMode::getFullscreenModes();
  bool fullscreen = false;
  unsigned antialiasing_level = 0;
  std::string title = "SNAKE GAME";
  sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
  unsigned framerate_limit = 60;
  bool vertical_sync_enabled = false;
  fullscreen = 0;
  antialiasing_level = 0;

  this->fullscreen = fullscreen;

  this->windowSettings.antialiasingLevel = antialiasing_level;

  if (this->fullscreen) {
    this->window = new sf::RenderWindow(window_bounds, title,
                                        sf::Style::Fullscreen, windowSettings);
  }

  else {
    this->window = new sf::RenderWindow(window_bounds, title,
                                        sf::Style::Titlebar | sf::Style::Close,
                                        windowSettings);
  }

  this->window->setFramerateLimit(framerate_limit);
  this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Game::initKeys() {

  this->supportedKeys["Escape"] = sf::Keyboard::Key::Escape;
  this->supportedKeys["A"] = sf::Keyboard::Key::A;
  this->supportedKeys["D"] = sf::Keyboard::Key::D;
  this->supportedKeys["W"] = sf::Keyboard::Key::W;
  this->supportedKeys["S"] = sf::Keyboard::Key::S;
  this->supportedKeys["P"] = sf::Keyboard::Key::P;

  for (auto i : this->supportedKeys) {
    std::cout << i.first << " " << i.second << "\n";
  }
}

void Game::initStates() {
  this->states.push(
      new MainMenu(this->window, &this->supportedKeys, &this->states));
}

// Costructors/Destructors
Game::Game() {
  this->initWindow();
  this->initKeys();
  this->initStates();
}

Game::~Game() {
  delete this->window;
  while (!this->states.empty()) {
    delete this->states.top();
    this->states.pop();
  }
}

// Functions

void Game::endApplication() {
  std::cout << "Ended"
            << "\n";
}
void Game::updateDt() { this->dt = this->dtClock.restart().asSeconds(); }

void Game::updateSFMLEvents() {
  while (this->window->pollEvent(this->sfEvent)) {
    if (this->sfEvent.type == sf::Event::Closed)
      this->window->close();
  }
}

void Game::update() {
  this->updateSFMLEvents();

  if (!this->states.empty()) {
    this->states.top()->update(this->dt);

    if (this->states.top()->getQuit()) {
      std::cout << "Yes" << std::endl;

      delete this->states.top();
      this->states.pop();
    }
  }

  else {
    this->endApplication();
    this->window->close();
  }
}

void Game::render() {
  this->window->clear();

  // Render items

  if (!this->states.empty())
    this->states.top()->render();

  this->window->display();
}

void Game::run() {

  sf::Clock clock;
  while (this->window->isOpen()) {

    this->updateDt();

    this->update();
    this->render();
  }
}
