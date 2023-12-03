#pragma once

#include <Rules/Rule.hpp>

namespace ink {

class Rule30 final : public Rule {
 public:
  using Rule::Rule;

  Cell::State getUpdatedState(std::size_t cell_idx) const noexcept override;
};

}  // namespace ink
