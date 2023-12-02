#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Cell.hpp>
#include <Rules/Rule.hpp>

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
  void handlePlayerInput(const sf::Event event);

 private:
  bool isBorderCell(sf::Vector2f cell_pos) const noexcept;
  Cell::State getUpdatedState(std::size_t cell_idx) noexcept;
  std::optional<std::size_t> findIntersectionBFS(
      const sf::Vector2f mouse_position);

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

  std::unique_ptr<Rule> rule_;
};

}  // namespace ink
