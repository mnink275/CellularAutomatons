#pragma once

#include <type_traits>

namespace ink::utils {

template <class EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
inline constexpr std::underlying_type_t<EnumType> toString(EnumType type) {
  return static_cast<std::underlying_type_t<EnumType>>(type);
}

}  // namespace ink::utils
