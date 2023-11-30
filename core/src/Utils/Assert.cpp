#include <Utils/Assert.hpp>

#include <cassert>
#include <iostream>

#include <fmt/core.h>

namespace ink::utils::impl {

void failedAssertMsg(std::string_view expr, const char* file, unsigned int line,
                     const char* function, std::string_view msg) noexcept {
  const auto kErrorMessage = fmt::format(
      "ERROR at {}:{}:{}.\nAssertion '{}' failed{}{}\n", file, line,
      (function ? function : ""), expr,
      (msg.empty() ? std::string_view{} : std::string_view{": "}), msg);

  std::cerr << fmt::format("{}", kErrorMessage) << std::flush;

  abort();
}

}  // namespace ink::utils::impl
