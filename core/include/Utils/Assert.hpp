#pragma once

#include <string_view>

namespace ink::utils::impl {

[[noreturn]] void failedAssertMsg(std::string_view expr, const char* file,
                                  unsigned int line, const char* function,
                                  std::string_view msg) noexcept;

#ifdef NDEBUG
inline constexpr auto kEnableAssert = false;
#else
inline constexpr auto kEnableAssert = true;
#endif

}  // namespace ink::utils::impl

#define ASSERT_MSG(expr, msg)                                                \
  if (ink::utils::impl::kEnableAssert) {                                     \
    if (!(expr)) {                                                           \
      ink::utils::impl::failedAssertMsg(#expr, __FILE__, __LINE__, __func__, \
                                        msg);                                \
    }                                                                        \
  }

#define ASSERT(expr) ASSERT_MSG(expr, {})
