#pragma once

#include <SFML/Graphics.hpp>

namespace ink {

class Cell : public sf::RectangleShape {
 public:
  enum class State {
    kActive,
    kInactive,
  };

 public:
  Cell(sf::RectangleShape&& rect);
  Cell(sf::RectangleShape&& rect, State state);

  void update(sf::Time dt);
  void SetState(State state);

 private:
  State state_;
  sf::Time timer_;
};

}  // namespace ink
