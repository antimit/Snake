#include "PauseState.h"

// Initializer functions
void PauseState::initVariables() {
  resolution = sf::Vector2f(1920, 1080);
  texture_Scroll = new sf::Texture;
  sprite_Scroll = new sf::Sprite[3];
  results = false;
}

void PauseState::initBackground() {
  this->background.setSize(
      sf::Vector2f(static_cast<float>(this->window->getSize().x),
                   static_cast<float>(this->window->getSize().x)));

  if (!this->backgroundTexture.loadFromFile(
          "../Resources/Images/Backgrounds/paused.jpg")) {
    throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
  }

  this->background.setTexture(&this->backgroundTexture);
}

void PauseState::initFonts() {
  if (!this->font.loadFromFile("../assets/Fonts/Dosis-Light.ttf")) {
    throw("ERROR::PauseStateSTATE::COULD NOT LOAD FONT");
  }
}

void PauseState::initKeybinds() {

  std::ifstream ifs("assets/Config/PauseStatestate_keybinds.ini");
  if (ifs.is_open()) {
    std::string key = "";
    std::string key2 = 0;

    while (ifs >> key >> key2) {
      this->keybinds[key] = this->supportedKeys->at(key2);
    }
  }
  ifs.close();

  this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");
}

void PauseState::initButtons() {

  this->buttons["Border"] = new Button(
      100, 780, 150, 50, &this->font, "Border", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

  this->buttons["Empty"] = new Button(
      500, 780, 150, 50, &this->font, "Empty", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

  this->buttons["EXIT_STATE"] = new Button(
      300, 780, 150, 50, &this->font, "QUIT", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

  this->buttons["RESULTS"] = new Button(
      1592, 99, 150, 50, &this->font, "Scores", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

  this->setSkin();
}

void PauseState::initSlider() {
  dragging = false;
  volume = 100.0;

  track.setSize(sf::Vector2f(200, 10));
  track.setPosition(300, 300);
  track.setFillColor(sf::Color(200, 200, 200));

  // Slider for the volume control
  slider.setSize(sf::Vector2f(20, 30));
  slider.setPosition(300, 290); // Initial position at the start of the track
  slider.setFillColor(sf::Color(100, 100, 250));
}

PauseState::PauseState(sf::RenderWindow *window,
                       std::map<std::string, int> *supportedKeys,
                       std::stack<State *> *states, sf::Music &musiC,
                       bool &border)
    : State(window, supportedKeys, states) {
  mus = &musiC;
  Border = &border;

  this->initVariables();
  this->initBackground();
  this->initFonts();
  this->initKeybinds();
  this->initButtons();
  this->windowElements();
  this->initSlider();
}

PauseState::~PauseState() {
  auto it = this->buttons.begin();
  for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
    delete it->second;
  }
}

void PauseState::setupText(Text *textItem, const Font &font,
                           const String &value, int size, Color colour) {
  textItem->setFont(font);
  textItem->setString(value);
  textItem->setCharacterSize(size);
  textItem->setFillColor(colour);
}

void PauseState::windowElements() {
  mainFont.loadFromFile("../assets/fonts/slant_regular.ttf");

  setupText(&pressEnterText, mainFont, "Select the Snake Skin", 38,
            Color::Magenta);
  FloatRect pressEnterTextBounds = pressEnterText.getLocalBounds();
  pressEnterText.setPosition(Vector2f(1002, 83));
  pressEnterText.setOutlineColor(Color::Black);
  pressEnterText.setOutlineThickness(2);


  setupText(&volumeText, mainFont, "Adjust the volume", 38,
            Color::Magenta);
  FloatRect volumeTextBounds = volumeText.getLocalBounds();
  volumeText.setPosition(Vector2f(360, 200));
  volumeText.setOutlineColor(Color::Black);
  volumeText.setOutlineThickness(2);
}

void PauseState::setSkin() {

  sf::Texture *texture1 = new sf::Texture();
  if (!texture1->loadFromFile("../Resources/Images/Icons/lethal.png")) {
    std::cerr << "Failed to load button image 1" << std::endl;
    delete texture1;
    texture1 = nullptr;
  }

  sf::Texture *texture2 = new sf::Texture();
  if (!texture2->loadFromFile("../Resources/Images/Icons/poison.jpg")) {
    std::cerr << "Failed to load button image 2" << std::endl;
    delete texture2;
    texture2 = nullptr;
  }

  this->buttons["POISON"] = new Button(
      1018, 199, 150, 50, &this->font, "", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
  if (texture1)
    this->buttons["POISON"]->SetTexture(
        texture1, true); // true to resize the button to the image size

  this->buttons["LETHAL"] = new Button(
      1118, 199, 150, 50, &this->font, "", sf::Color(100, 100, 100, 200),
      sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
  if (texture2)
    this->buttons["LETHAL"]->SetTexture(texture2, true);
}

std::vector<std::pair<int, int>> PauseState::fetchHighScores() {
  sqlite3 *db;
  char *errMsg = nullptr;
  std::vector<std::pair<int, int>> scores;

  if (sqlite3_open(DATABASE_PATH.c_str(), &db) == SQLITE_OK) {
    const char *query =
        "SELECT ID, Score FROM HighScores ORDER BY Score DESC LIMIT 10;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
      while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);
        scores.push_back(std::make_pair(id, score));
      }
      sqlite3_finalize(stmt);
    } else {
      std::cerr << "Failed to fetch scores: " << sqlite3_errmsg(db)
                << std::endl;
    }
    sqlite3_close(db);
  } else {
    std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
  }

  return scores;
}

void PauseState::updateInput(const float &dt) {
  // this->checkForQuit();
}

void PauseState::updateButtons() {

  for (auto &it : this->buttons) {
    it.second->update(this->mousePosView);
  }

  // Quit the game
  if (this->buttons["EXIT_STATE"]->isPressed()) {
    this->endState();
  }

  if (this->buttons["Border"]->isPressed()) {
    *Border = true;
  }

  if (this->buttons["Empty"]->isPressed()) {
    *Border = false;
  }

  if (this->buttons["RESULTS"]->isPressed()) {
    results = true;
  }

  if (this->buttons["POISON"]->isPressed()) {
    GameEvent::Event e{"ChangeSkin",
                       "../assets/Player/Textures/poisonbody.png"};
    GameEvent::EventManager::getInstance().publish(e);
  }

  // Handle skin change to "Lethal"
  if (this->buttons["LETHAL"]->isPressed()) {
    GameEvent::Event e{"ChangeSkin",
                       "../assets/Player/Textures/lethalbody.png"};
    GameEvent::EventManager::getInstance().publish(e);
  }
}

void PauseState::update(const float &dt) {
  this->updateMousePositions();
  this->updateInput(dt);
  this->updateButtons();

  if (this->dragging && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    float mouseX = static_cast<float>(sf::Mouse::getPosition(*this->window).x);
    float newPos = std::max(mouseX, track.getPosition().x);
    newPos = std::min(newPos, track.getPosition().x + track.getSize().x -
                                  slider.getSize().x);
    slider.setPosition(newPos, slider.getPosition().y);

    volume = 100 * ((newPos - track.getPosition().x) /
                    (track.getSize().x - slider.getSize().x));

    this->mus->setVolume(volume);
  }

  // Check if the slider is being grabbed
  if (!dragging && sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
      slider.getGlobalBounds().contains(
          static_cast<float>(sf::Mouse::getPosition(*this->window).x),
          static_cast<float>(sf::Mouse::getPosition(*this->window).y))) {
    dragging = true;
  } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    dragging = false;
  }
}

void PauseState::renderButtons(sf::RenderTarget *target) {

  for (auto &it : this->buttons) {
    it.second->render(target);
  }
}

void PauseState::render(sf::RenderTarget *target) {
  if (!target)
    target = this->window;

  target->draw(this->background);
  if (results) {
    std::vector<std::pair<int, int>> highScores = fetchHighScores();
    sf::Text scoreText;
    scoreText.setFont(this->font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    float yOffset = 220;
    for (auto &score : highScores) {
      std::stringstream ss;
      ss << "ID " << score.first << ": " << score.second;
      scoreText.setString(ss.str());
      scoreText.setPosition(1608, yOffset);
      yOffset += 30;
      target->draw(scoreText);
    }
  }

  target->draw(pressEnterText);
  target->draw(volumeText);

  this->renderButtons(target);
  target->draw(track);
  target->draw(slider);

  sf::Text mouseText;
  mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
  mouseText.setFont(this->font);
  mouseText.setCharacterSize(12);
  std::stringstream ss;
  ss << this->mousePosView.x << " " << this->mousePosView.y;

  mouseText.setString(ss.str());

  target->draw(mouseText);
}