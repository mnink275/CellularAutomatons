#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

#include <Rules/Rule.hpp>

namespace ink {

enum class Neighborhood {
  kMoore,
  kVonNeumann,
};

template <Neighborhood NHValue>
class Rule110 : public Rule {
 public:
  Rule110(const std::vector<Cell>& field, const std::size_t row_size)
      : Rule(field, row_size) {}

  virtual Cell::State getUpdatedState(std::size_t cell_idx) const noexcept {
    // Rule 110 CA implementaion
    auto& cell = field_[cell_idx];
    auto cell_state = cell.getState();
    if (cell_state == Cell::State::kBorder) return cell_state;

    auto neighborhood_indexes = getNeighborhoodArray(cell_idx);
    std::size_t alive_count =
        std::count_if(neighborhood_indexes.begin(), neighborhood_indexes.end(),
                      [this](std::size_t idx) {
                        return field_[idx].getState() == Cell::State::kActive;
                      });

    if (cell_state == Cell::State::kActive &&
        (alive_count < 2 || alive_count > 3)) {
      return Cell::State::kInactive;
    }
    if (cell_state == Cell::State::kInactive && alive_count == 3) {
      return Cell::State::kActive;
    }

    return cell_state;
  }

  auto getNeighborhoodArray(std::size_t cell_idx) const noexcept {
    if constexpr (NHValue == Neighborhood::kMoore) {
      // Moore neighborhood
      return std::array<std::size_t, 8>{
          // top cells
          cell_idx - kRowSize_ - 1, cell_idx - kRowSize_,
          cell_idx - kRowSize_ + 1,
          // left and right cells
          cell_idx - 1, cell_idx + 1,
          // bottom cells
          cell_idx + kRowSize_ - 1, cell_idx + kRowSize_,
          cell_idx + kRowSize_ + 1};
    } else {
      // Von Neumann neighborhood
      return std::array<std::size_t, 4>{
          cell_idx - kRowSize_,  // top cell
          cell_idx - 1,          // left cell
          cell_idx + 1,          // right cells
          cell_idx + kRowSize_   // bottom cell
      };
    }
  }
};

}  // namespace ink
