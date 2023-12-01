#include <World.hpp>

#include <array>
#include <cmath>
#include <functional>

#include <fmt/format.h>

#include <Utils/Assert.hpp>

namespace ink {

namespace {

// TODO: Is there a better option to find the "border cells"?
auto BorderRectInit = [](const float cell_size, const float cell_size_eps,
                         const sf::FloatRect world_bounds) {
  return sf::FloatRect{
      {0.0f, 0.0f},
      {std::floor((world_bounds.width - cell_size_eps) / cell_size) * cell_size,
       std::floor((world_bounds.height - cell_size_eps) / cell_size) *
           cell_size}};
};

}  // namespace

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}),
      field_(),
      updated_field_(),
      kCellSize_(20.f),
      kCellSizeEps_(0.1f * kCellSize_),
      kRowSize_(std::floor(world_bounds_.width / kCellSize_)),
      kColumnSize_(std::floor(world_bounds_.height / kCellSize_)),
      kBorderRect_(std::invoke(BorderRectInit, kCellSize_, kCellSizeEps_,
                               world_bounds_)),
      timer_(sf::Time::Zero) {
  ASSERT(kBorderRect_.width > 0.0f);
  ASSERT(kBorderRect_.height > 0.0f);
  const auto kOutlineThickness = 0.5f;
  field_.reserve(kRowSize_ * kColumnSize_);
  for (auto y = kBorderRect_.top; y < kBorderRect_.height + kCellSizeEps_;
       y += kCellSize_) {
    for (auto x = kBorderRect_.left; x < kBorderRect_.width + kCellSizeEps_;
         x += kCellSize_) {
      sf::RectangleShape rect{{kCellSize_, kCellSize_}};
      rect.setPosition({x, y});
      rect.setOutlineColor(sf::Color{128, 128, 128});  // Grey
      rect.setOutlineThickness(-kOutlineThickness);

      auto default_state = Cell::State::kInactive;
      if (isBorderCell({x, y})) {
        default_state = Cell::State::kBorder;
      }
      field_.emplace_back(std::move(rect), default_state);
    }
  }
  updated_field_ = field_;
}

void World::update(const sf::Time dt) {
  static constexpr auto kStateChangeDuration = sf::seconds(0.1f);
  timer_ += dt;
  if (timer_ >= kStateChangeDuration) {
    timer_ -= kStateChangeDuration;

    for (std::size_t i = 0; i < field_.size(); ++i) {
      updated_field_[i].setState(getUpdatedState(i));
    }
    std::swap(field_, updated_field_);
  }
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
    if (cell.getState() == Cell::State::kBorder ||
        !cell.getGlobalBounds().contains(kMousePosition))
      continue;
    cell.setState(Cell::State::kActive);
  }
}

bool World::isBorderCell(sf::Vector2f cell_pos) const noexcept {
  static constexpr float kEps = 1e-5;
  return std::abs(cell_pos.x - kBorderRect_.left) < kEps ||
         std::abs(cell_pos.x - kBorderRect_.width) < kEps ||
         std::abs(cell_pos.y - kBorderRect_.top) < kEps ||
         std::abs(cell_pos.y - kBorderRect_.height) < kEps;
}

Cell::State World::getUpdatedState(std::size_t cell_idx) noexcept {
  auto& cell = field_[cell_idx];
  auto cell_state = cell.getState();
  if (cell_state == Cell::State::kBorder) return cell_state;

  // Moore neighborhood
  std::array<std::size_t, 8> neighborhood_indexes{
      // top cells
      cell_idx - kRowSize_ - 1, cell_idx - kRowSize_, cell_idx - kRowSize_ + 1,
      // left and right cells
      cell_idx - 1, cell_idx + 1,
      // bottom cells
      cell_idx + kRowSize_ - 1, cell_idx + kRowSize_, cell_idx + kRowSize_ + 1};
  std::size_t alive_count =
      std::count_if(neighborhood_indexes.begin(), neighborhood_indexes.end(),
                    [this](std::size_t idx) {
                      return field_[idx].getState() == Cell::State::kActive;
                    });

  if (cell_state == Cell::State::kActive &&
      (alive_count < 2 || alive_count > 3)) {
    return Cell::State::kInactive;
  }
  if (cell_state == Cell::State::kInactive && alive_count == 3) {
    return Cell::State::kActive;
  }

  return cell_state;
}

}  // namespace ink
