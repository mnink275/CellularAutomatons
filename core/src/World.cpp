#include <World.hpp>

#include <fmt/format.h>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}) {
//  world_view_.setCenter(spawn_position_);
}

void World::update(const sf::Time dt) {
  
}

void World::draw() const {
  window_.setView(world_view_);
  //window_.draw(scene_graph_);
}

void World::handlePlayerInput(const sf::Keyboard::Key key,
                              const bool is_pressed) {
  switch (key) {
    case sf::Keyboard::Key::A:
    case sf::Keyboard::Key::D:
    case sf::Keyboard::Key::W:
    case sf::Keyboard::Key::S:
    case sf::Keyboard::Key::E:
    case sf::Keyboard::Key::Space:
      break;
    default:
      fmt::println("The key wasn't implemented");
  }
}

}  // namespace ink
