#include <World.hpp>

#include <cmath>
#include <functional>
#include <queue>

#include <fmt/format.h>

#include <Buttons/RuleActivateButton.hpp>
#include <Rules/Rule110.hpp>
#include <Rules/Rule30.hpp>
#include <Rules/Rule60.hpp>
#include <Utils/Assert.hpp>

namespace ink {

namespace {

// TODO: Is there a better option to find the "border cells"?
const auto borderCellsRectIniter = [](const float cell_size,
                                      const float cell_size_eps,
                                      const sf::FloatRect& world_bounds) {
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
      kCellSize_(20.0f),
      kCellSizeEps_(0.1f * kCellSize_),
      kRowSize_(std::floor(world_bounds_.width / kCellSize_)),
      kColumnSize_(std::floor(world_bounds_.height / kCellSize_)),
      kBorderCellsRect_(std::invoke(borderCellsRectIniter, kCellSize_,
                                    kCellSizeEps_, world_bounds_)),
      timer_(sf::Time::Zero),
      font_(),
      rules_holder_(),
      active_rule_(nullptr) {
  auto font_file_path = std::string{RESOURCE_FOLDER} + "/Expressway.ttf";
  if (!font_.loadFromFile(font_file_path)) {
    throw std::runtime_error("Failed to load " + font_file_path);
  }
  ASSERT(kBorderCellsRect_.width > 0.0f);
  ASSERT(kBorderCellsRect_.height > 0.0f);
  const auto kOutlineThickness = 0.5f;
  field_.reserve(kRowSize_ * kColumnSize_);
  for (auto y = kBorderCellsRect_.top;
       y < kBorderCellsRect_.height + kCellSizeEps_; y += kCellSize_) {
    for (auto x = kBorderCellsRect_.left;
         x < kBorderCellsRect_.width + kCellSizeEps_; x += kCellSize_) {
      sf::RectangleShape rect{{kCellSize_, kCellSize_}};
      rect.setPosition({x, y});
      rect.setOutlineColor(sf::Color{192, 192, 192});  // Grey
      rect.setOutlineThickness(-kOutlineThickness);

      auto default_state = Cell::State::kInactive;
      if (isBorderCell({x, y})) {
        default_state = Cell::State::kBorder;
      }
      field_.emplace_back(std::move(rect), default_state);
    }
  }
  updated_field_ = field_;

  initRules();
  initButtons();
}

void World::update(const sf::Time dt) {
  static constexpr auto kStateChangeDuration = sf::seconds(1 / 60.0f);
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
  for (auto&& button : buttons_holder_) {
    window_.draw(*button);
  }
}

void World::handlePlayerInput(const sf::Event event) {
  auto new_state = Cell::State::kNone;
  auto mouse_position = sf::Vector2f{};
  switch (event.type) {
    case sf::Event::MouseMoved:
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        new_state = Cell::State::kActive;
      } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        new_state = Cell::State::kInactive;
      }
      mouse_position.x = static_cast<float>(event.mouseMove.x);
      mouse_position.y = static_cast<float>(event.mouseMove.y);
      break;
    case sf::Event::MouseButtonPressed:
      if (event.mouseButton.button == sf::Mouse::Left) {
        new_state = Cell::State::kActive;
      } else if (event.mouseButton.button == sf::Mouse::Right) {
        new_state = Cell::State::kInactive;
      }
      mouse_position.x = static_cast<float>(event.mouseButton.x);
      mouse_position.y = static_cast<float>(event.mouseButton.y);
      break;
    default:
      return;
  }

  if (new_state == Cell::State::kNone) return;

  for (auto&& button : buttons_holder_) {
    if (button->contains(mouse_position)) {
      resetFields();
      active_rule_ = rules_holder_[button->getRuleNumber()].get();
      return;
    }
  }

  auto cell_idx = findIntersectionBFS(mouse_position);
  if (cell_idx.has_value()) {
    field_[cell_idx.value()].setState(new_state);
  }
}

bool World::isBorderCell(sf::Vector2f cell_pos) const noexcept {
  static constexpr float kEps = 1e-5;
  return std::abs(cell_pos.x - kBorderCellsRect_.left) < kEps ||
         std::abs(cell_pos.x - kBorderCellsRect_.width) < kEps ||
         std::abs(cell_pos.y - kBorderCellsRect_.top) < kEps ||
         std::abs(cell_pos.y - kBorderCellsRect_.height) < kEps;
}

Cell::State World::getUpdatedState(std::size_t cell_idx) noexcept {
  return active_rule_->getUpdatedState(cell_idx);
}

std::optional<std::size_t> World::findIntersectionBFS(
    const sf::Vector2f mouse_position) {
  // Running the search bfs algorithm from the last touched cell
  // There is a high probability that the cursor is nearby
  static std::size_t last_touched_cell_idx =
      kRowSize_ + 1;  // top-left non-border cell

  // TODO: optimize memory usage
  std::vector<std::size_t> is_visited(field_.size(), 0);
  std::queue<std::size_t> queue;
  queue.push(last_touched_cell_idx);
  while (!queue.empty()) {
    auto cell_idx = queue.front();
    queue.pop();
    if (is_visited[cell_idx] != 0) continue;
    is_visited[cell_idx] = 1;
    auto& cell = field_[cell_idx];
    if (cell.getState() == Cell::State::kBorder) continue;
    if (cell.getGlobalBounds().contains(mouse_position)) {
      last_touched_cell_idx = cell_idx;
      return cell_idx;
    }

    queue.push(cell_idx - 1);
    queue.push(cell_idx + 1);
    queue.push(cell_idx - kRowSize_);
    queue.push(cell_idx + kRowSize_);
  }

  return std::nullopt;
}

void World::initRules() {
  rules_holder_[RuleNumber::k110] =
      std::make_unique<Rule110<Neighborhood::kMoore>>(field_, kRowSize_);
  rules_holder_[RuleNumber::k60] = std::make_unique<Rule60>(field_, kRowSize_);
  rules_holder_[RuleNumber::k30] = std::make_unique<Rule30>(field_, kRowSize_);

  active_rule_ = rules_holder_[RuleNumber::k110].get();
}

void World::initButtons() {
  const auto start_pos = world_bounds_.getPosition() * 0.8f;
  auto button_rect = sf::RectangleShape{{60.0f, 30.0f}};
  button_rect.setFillColor(sf::Color::Black);

  button_rect.setPosition(start_pos);
  buttons_holder_.emplace_back(std::make_unique<RuleActivateButton>(
      font_, "Rule 110", button_rect, RuleNumber::k110));

  button_rect.setPosition(start_pos + sf::Vector2f{0.0f, 40.0f});
  buttons_holder_.emplace_back(std::make_unique<RuleActivateButton>(
      font_, "Rule 60", button_rect, RuleNumber::k60));

  button_rect.setPosition(start_pos + sf::Vector2f{0.0f, 80.0f});
  buttons_holder_.emplace_back(std::make_unique<RuleActivateButton>(
      font_, "Rule 30", button_rect, RuleNumber::k30));
}

void World::resetFields() {
  for (auto&& cell : field_) {
    if (cell.getState() == Cell::State::kBorder) continue;
    cell.setState(Cell::State::kInactive);
  }
  updated_field_ = field_;
}

}  // namespace ink
