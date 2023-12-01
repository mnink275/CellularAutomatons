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

void World::handlePlayerInput(const sf::Event::MouseButtonEvent key,
                              const bool is_pressed) {
  if (!is_pressed) return;

  const auto kMousePoint =
      sf::Vector2f{static_cast<float>(key.x), static_cast<float>(key.y)};
  switch (key.button) {
    case sf::Mouse::Button::Left:
      for (auto&& cell : field_) {
        if (!cell.getGlobalBounds().contains(kMousePoint)) continue;
        cell.setFillColor(sf::Color::Green);
      }
      break;
    default:
      fmt::println("The key wasn't implemented");
  }
}

}  // namespace ink
