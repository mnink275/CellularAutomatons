#include <World.hpp>

#include <fmt/format.h>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}) {
  const auto kCellSize = 20.0f;
  const auto kOutlineThickness = 1.0f;
  for (auto y = 0.0f; y < world_bounds_.height; y += kCellSize) {
    for (auto x = 0.0f; x < world_bounds_.width; x += kCellSize) {
      sf::RectangleShape rect{{kCellSize, kCellSize}};
      rect.setPosition({x, y});
      rect.setFillColor(sf::Color::White);
      rect.setOutlineColor(sf::Color::Black);
      rect.setOutlineThickness(-kOutlineThickness);

      field_.emplace_back(std::move(rect));
    }
  }
}

void World::update(const sf::Time dt) {
  for (auto&& cell : field_) cell.update(dt);
}

void World::draw() const {
  window_.setView(world_view_);
  for (auto&& cell : field_) {
    window_.draw(cell);
  }
}

void World::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return;

  const auto kMousePosition =
      sf::Vector2f{static_cast<float>(event.x), static_cast<float>(event.y)};
  for (auto&& cell : field_) {
    if (!cell.getGlobalBounds().contains(kMousePosition)) continue;
    cell.SetState(Cell::State::kActive);
  }
}

}  // namespace ink
