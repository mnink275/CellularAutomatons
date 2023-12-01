#pragma once

#include <SFML/Graphics.hpp>

namespace ink {

class Cell : public sf::RectangleShape {
 public:
  enum class State;

  Cell(sf::RectangleShape&& rect);
  Cell(sf::RectangleShape&& rect, State state);

  void update(sf::Time dt);
  void SetState(State state);
  State GetState() const noexcept;

 public:
  enum class State {
    kNone,
    kActive,
    kInactive,
    kBorder,
  };

 private:
  void UpdateState();

 private:
  State state_;
  sf::Time timer_;
};

}  // namespace ink
