#pragma once

#include <SFML/Graphics.hpp>

namespace ink {

class Cell final : public sf::RectangleShape {
 public:
  enum class [[nodiscard]] State{
      kNone,
      kActive,
      kInactive,
      kBorder,
  };

  explicit Cell(sf::RectangleShape&& rect);
  Cell(sf::RectangleShape&& rect, State state);

  void setState(State state);
  State getState() const noexcept;

 private:
  State state_;
};

}  // namespace ink
