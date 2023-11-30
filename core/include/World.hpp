#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace ink {

class World final {
 public:
  explicit World(sf::RenderWindow& window);

  ~World() = default;

  World(const World&) = delete;
  World& operator=(const World&) = delete;

  World(World&&) = delete;
  World& operator=(World&&) = delete;

  void update(sf::Time dt);
  void draw() const;
  void handlePlayerInput(sf::Keyboard::Key key, bool is_pressed);

 private:
  sf::RenderWindow& window_;
  sf::View world_view_;
  sf::FloatRect world_bounds_;

  std::vector<std::vector<int>> field_;
};

}  // namespace ink
