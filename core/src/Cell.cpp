#include <Cell.hpp>

#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace ink {

namespace {

// TODO: change to std::array for better performance
const std::unordered_map<Cell::State, sf::Color> kStateColors{
    {Cell::State::kActive, sf::Color::Green},
    {Cell::State::kInactive, sf::Color::Black},
    {Cell::State::kBorder, sf::Color::Blue},
};

}  // namespace

Cell::Cell(sf::RectangleShape&& rect)
    : Cell(std::move(rect), State::kInactive) {}

Cell::Cell(sf::RectangleShape&& rect, State state)
    : sf::RectangleShape(std::move(rect)), state_(State::kNone) {
  SetState(state);
}

void Cell::SetState(State state) {
  state_ = state;
  setFillColor(kStateColors.at(state));
}

Cell::State Cell::GetState() const noexcept { return state_; }

void Cell::ChangeState() {
  switch (state_) {
    case State::kActive:
      SetState(State::kInactive);
      break;
    case State::kInactive:
      SetState(State::kActive);
      break;
    default:
      break;
  }
}

}  // namespace ink
