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
  bool isBorderCell(sf::Vector2f cell_pos) const noexcept;
  Cell::State getUpdatedState(std::size_t cell_idx) noexcept;

 private:
  sf::RenderWindow& window_;
  sf::View world_view_;
  sf::FloatRect world_bounds_;

  std::vector<Cell> field_;
  std::vector<Cell> updated_field_;

  const float kCellSize_;
  const float kCellSizeEps_;
  const std::size_t kRowSize_;
  const std::size_t kColumnSize_;
  const sf::FloatRect kBorderRect_;
  sf::Time timer_;
};

}  // namespace ink
