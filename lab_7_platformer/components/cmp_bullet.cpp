#include "cmp_bullet.h"
#include "LevelSystem.h" // Required for tile checks
#include <iostream>      // For debugging

using namespace std;
using namespace sf;

void BulletComponent::update(double dt) {
    // Decrease the lifetime of the bullet
    _lifetime -= dt;

    // Check for wall collision or out-of-bounds
    if (checkWallCollision()) {
        _parent->setForDelete(); // Mark the bullet for deletion
        return; // Exit early
    }

        // Check lifetime expiration
   if (_lifetime <= 0.f) {
            _parent->setForDelete();
            return;
        }
    }


BulletComponent::BulletComponent(Entity* p, float lifetime)
    : Component(p), _lifetime(lifetime) {}

bool BulletComponent::checkWallCollision() const {
    if (!_parent) {
        return false;
    }

    sf::Vector2f position = _parent->getPosition();

    // Determine direction of the bullet based on velocity or movement
    sf::Vector2f direction(1.0f, 0.0f); // Replace with the actual bullet direction if available

    // Adjust the position slightly in the direction of travel
    const float checkOffset = 5.f; // Tune this value based on bullet size and speed
    sf::Vector2f checkPosition = position + direction * checkOffset;

    // Debug log
    std::cout << "Bullet position: (" << position.x << ", " << position.y << ")\n";
    std::cout << "Adjusted check position: (" << checkPosition.x << ", " << checkPosition.y << ")\n";

    if (!ls::isOnGrid(checkPosition)) {
        return false;
    }

    try {
        auto tile = ls::getTileAt(checkPosition);
        std::cout << "Tile at check position: " << static_cast<int>(tile) << "\n";

        if (tile == LevelSystem::WALL) {
            std::cout << "Collision with wall detected!\n";
            return true;
        }
    } catch (const std::string& e) {
        std::cout << "Error: " << e << "\n";
    }

    return false;
}