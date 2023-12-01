#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include <Cell.hpp>

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
  void handlePlayerInput(const sf::Event::MouseMoveEvent event);

 private:
  sf::RenderWindow& window_;
  sf::View world_view_;
  sf::FloatRect world_bounds_;

  std::vector<Cell> field_;
};

}  // namespace ink
