#pragma once

#include <Rules/Rule.hpp>

namespace ink {

class Rule30 : public Rule {
 public:
  using Rule::Rule;

  virtual Cell::State getUpdatedState(std::size_t cell_idx) const noexcept;
};

}  // namespace ink
