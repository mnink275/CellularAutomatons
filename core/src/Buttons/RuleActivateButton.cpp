#include <Buttons/RuleActivateButton.hpp>

namespace ink {

RuleActivateButton::RuleActivateButton(const sf::Font& font,
                                       const std::string& name,
                                       sf::RectangleShape rect,
                                       RuleNumber rule_number)
    : Button(font, name, std::move(rect)), rule_number_(rule_number) {}

RuleNumber RuleActivateButton::getRuleNumber() const noexcept {
  return rule_number_;
}

}  // namespace ink
