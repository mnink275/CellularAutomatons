#include <Game.hpp>

#include <stdexcept>

namespace ink {

namespace {

constexpr auto kResolution = sf::Vector2u{1920, 1080};

}  // namespace

Game::Game()
    : kTimePerFrame(sf::seconds(1 / 60.0f)),
      window_(sf::VideoMode(kResolution), "Cellar Automatons",
              sf::Style::Close),
      world_(window_) {
  // window_.setPosition({0, 0});
}

void Game::run() {
  sf::Clock clock{};
  auto time_since_last_update = sf::Time::Zero;
  while (window_.isOpen()) {
    processEvents();
    time_since_last_update += clock.restart();
    while (time_since_last_update > kTimePerFrame) {
      time_since_last_update -= kTimePerFrame;
      processEvents();
      update(kTimePerFrame);
      render();
    }
  }
}

void Game::processEvents() {
  sf::Event event{};
  while (window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window_.close();
        break;
      default:
        world_.handlePlayerInput(event);
        break;
    }
  }
}

void Game::update(const sf::Time elapsed_time) { world_.update(elapsed_time); }

void Game::render() {
  window_.clear();
  world_.draw();
  window_.display();
}

}  // namespace ink
