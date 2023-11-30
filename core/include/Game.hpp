#pragma once

#include <SFML/Graphics.hpp>

#include <World.hpp>

namespace ink {

class Game final {
 public:
  Game();

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  Game(Game&&) = delete;
  Game& operator=(Game&&) = delete;

  ~Game() = default;

  void run();

 private:
  void processEvents();
  void update(sf::Time elapsed_time);
  void render();

 private:
  const sf::Time kTimePerFrame;
  sf::RenderWindow window_;
  World world_;
};

}  // namespace ink
