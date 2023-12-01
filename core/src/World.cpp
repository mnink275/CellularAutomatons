#include <World.hpp>

#include <fmt/format.h>
#include <cmath>

#include <Utils/Assert.hpp>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}) {
  const auto kCellSize = 20.f;
  const auto kCellSizeEps = 0.1f * kCellSize;
  const auto kOutlineThickness = 1.0f;
  // TODO: Is there a better option to find the "border cells"?
  static constexpr float kEps = 1e-5;
  const auto kBorderRect = sf::FloatRect{
      {0.0f, 0.0f},
      {std::floor((world_bounds_.width - kCellSizeEps) / kCellSize) * kCellSize,
       std::floor((world_bounds_.height - kCellSizeEps) / kCellSize) *
           kCellSize}};
  ASSERT(kBorderRect.width > 0.0f);
  ASSERT(kBorderRect.height > 0.0f);

  auto IsBorderCell = [&kBorderRect](float x, float y) {
    return std::abs(x - kBorderRect.left) < kEps ||
           std::abs(x - kBorderRect.width) < kEps ||
           std::abs(y - kBorderRect.top) < kEps ||
           std::abs(y - kBorderRect.height) < kEps;
  };
  for (auto y = kBorderRect.top; y < kBorderRect.height + kCellSizeEps;
       y += kCellSize) {
    for (auto x = kBorderRect.left; x < kBorderRect.width + kCellSizeEps;
         x += kCellSize) {
      fmt::println("x: {}, y: {}", x, y);
      sf::RectangleShape rect{{kCellSize, kCellSize}};
      rect.setPosition({x, y});
      rect.setOutlineColor(sf::Color::Black);
      rect.setOutlineThickness(-kOutlineThickness);

      auto default_state = Cell::State::kInactive;
      if (IsBorderCell(x, y)) {
        default_state = Cell::State::kBorder;
      }
      field_.emplace_back(std::move(rect), default_state);
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
    if (cell.GetState() == Cell::State::kBorder ||
        !cell.getGlobalBounds().contains(kMousePosition))
      continue;
    cell.SetState(Cell::State::kActive);
  }
}

}  // namespace ink
