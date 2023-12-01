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
  setState(state);
}

void Cell::setState(State state) {
  state_ = state;
  setFillColor(kStateColors.at(state));
}

Cell::State Cell::getState() const noexcept { return state_; }

}  // namespace ink
