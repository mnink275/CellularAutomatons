#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace ink {

class Button : public sf::Drawable {
 public:
  Button(const sf::Font& font, const std::string& name,
         sf::RectangleShape rect);

  void draw(sf::RenderTarget& target,
            const sf::RenderStates& states) const final;
  void update(sf::Time dt);
  bool contains(const sf::Vector2f point) const noexcept;

 private:
  sf::Text text_;
  const sf::RectangleShape rect_;
};

}  // namespace ink
