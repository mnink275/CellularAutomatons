#pragma once

#include <Buttons/Button.hpp>
#include <Rules/RuleTypes.hpp>

namespace ink {

class RuleActivateButton : public Button {
 public:
  RuleActivateButton(const sf::Font& font, std::string name,
                     sf::RectangleShape rect, RuleNumber rule_number);

  RuleNumber getRuleNumber() const noexcept;

 private:
  RuleNumber rule_number_;
};

}  // namespace ink
