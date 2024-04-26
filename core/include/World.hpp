#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Buttons/RuleActivateButton.hpp>
#include <Cell.hpp>
#include <Rules/Rule.hpp>
#include <Rules/RuleTypes.hpp>

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

  void initRules();
  void initButtons();

  void resetFields();

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
  const sf::FloatRect kBorderCellsRect_;
  sf::Time timer_;
  sf::Font font_;

  using RuleUPtr = std::unique_ptr<Rule>;
  std::unordered_map<RuleNumber, RuleUPtr> rules_holder_;
  Rule* active_rule_;

  using ButtonUPtr = std::unique_ptr<RuleActivateButton>;
  std::vector<ButtonUPtr> buttons_holder_;
};

}  // namespace ink
