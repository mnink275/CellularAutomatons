#include <fmt/core.h>
#include <SFML/Graphics.hpp>

#include <Game.hpp>

int main() {
  try {
    ink::Game game;
    game.run();
  } catch (std::exception& e) {
    fmt::println("Exception: ", e.what());
  }
}
