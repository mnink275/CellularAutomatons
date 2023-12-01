#include <Cell.hpp>

#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace ink {

namespace {

// TODO: change to std::array for better performance
const std::unordered_map<Cell::State, sf::Color> kStateColors{
    {Cell::State::kActive, sf::Color::Green},
    {Cell::State::kInactive, sf::Color::White},
};

}  // namespace

Cell::Cell(sf::RectangleShape&& rect)
    : Cell(std::move(rect), State::kInactive) {}

Cell::Cell(sf::RectangleShape&& rect, State state)
    : sf::RectangleShape(std::move(rect)),
      state_(state),
      timer_(sf::Time::Zero) {}

void Cell::update(sf::Time dt) {
  static constexpr auto kStateChangeDuration = sf::seconds(1.0f);
  timer_ += dt;
  if (timer_ >= kStateChangeDuration) {
    timer_ -= kStateChangeDuration;
    // TODO: move to ChangeState method
    switch (state_) {
      case State::kActive:
        state_ = State::kInactive;
        break;
      case State::kInactive:
        state_ = State::kActive;
        break;
    }
    setFillColor(kStateColors.at(state_));
  }
}

}  // namespace ink
