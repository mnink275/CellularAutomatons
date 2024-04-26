#include <Rules/Rule60.hpp>

#include <array>
#include <bitset>

namespace ink {

Cell::State Rule60::getUpdatedState(std::size_t cell_idx) const noexcept {
  // Rule 60 CA implementaion
  const auto& cell = field_[cell_idx];
  auto cell_state = cell.getState();
  if (cell_state == Cell::State::kBorder || cell_state == Cell::State::kActive)
    return cell_state;

  std::bitset<3> bits;
  std::array<std::size_t, 3> neighborhood_indexes{
      // 3 top cells, right to left
      cell_idx - kRowSize_ + 1, cell_idx - kRowSize_, cell_idx - kRowSize_ - 1};
  for (std::size_t i = 0; i < bits.size(); ++i) {
    auto idx = neighborhood_indexes[i];
    bits[i] = field_[idx].getState() == Cell::State::kActive;
  }

  if (bits == 0b101 || bits == 0b100 || bits == 0b011 || bits == 0b010) {
    return Cell::State::kActive;
  }

  return Cell::State::kInactive;
}

}  // namespace ink
