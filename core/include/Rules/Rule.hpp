#pragma once

#include <cstddef>
#include <vector>

#include <Cell.hpp>

namespace ink {

class Rule {
 public:
  Rule(const std::vector<Cell>& field, const std::size_t row_size)
      : field_(field), kRowSize_(row_size) {}

  Rule(const Rule&) = delete;
  Rule(Rule&&) = default;
  Rule& operator=(const Rule&) = delete;
  Rule& operator=(Rule&&) = default;

  virtual ~Rule() = default;

  virtual Cell::State getUpdatedState(std::size_t cell_idx) const noexcept = 0;

 protected:
  const std::vector<Cell>& field_;
  const std::size_t kRowSize_;
};

}  // namespace ink
