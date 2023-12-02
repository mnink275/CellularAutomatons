#pragma once

namespace ink {

enum class RuleNumber {
  k30,
  k60,
  k110,
  kRuleNumberCount,
};

inline constexpr auto kRuleNumberCount =
    static_cast<int>(RuleNumber::kRuleNumberCount);

}  // namespace ink
