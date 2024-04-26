#pragma once

#include <Buttons/Button.hpp>
#include <Rules/RuleTypes.hpp>

namespace ink {

class RuleActivateButton final : public Button {
 public:
  RuleActivateButton(const sf::Font& font, const std::string& name,
                     sf::RectangleShape rect, RuleNumber rule_number);

  RuleNumber getRuleNumber() const noexcept;

 private:
  RuleNumber rule_number_;
};

}  // namespace ink
