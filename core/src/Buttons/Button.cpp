#include <Buttons/Button.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

namespace ink {

Button::Button(const sf::Font& font, std::string name, sf::RectangleShape rect)
    : text_(font, std::move(name)), rect_(std::move(rect)) {
  text_.setPosition(rect_.getPosition());
  text_.setCharacterSize(20);
}

void Button::draw(sf::RenderTarget& target,
                  const sf::RenderStates& states) const {
  target.draw(rect_, states);
  target.draw(text_, states);
}

void Button::update(sf::Time /*dt*/) {}

bool Button::contains(const sf::Vector2f point) const noexcept {
  return rect_.getGlobalBounds().contains(point);
}

}  // namespace ink
