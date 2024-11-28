#include "cmp_player_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

bool isFacingLeft;

PlayerTurretComponent::PlayerTurretComponent(Entity* p)
    : Component(p), _firetime(0.f), shootCooldown(0.5f) {}  // Initialize _firetime and shootCooldown

void PlayerTurretComponent::update(double dt) {
    // Update fire time
    _firetime += dt;

    // Check if left or right key is pressed to update the facing direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        isFacingLeft = true;  // Player is facing left
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        isFacingLeft = false;  // Player is facing right
    }

    // Handle other input actions (firing)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        // Fire if cooldown has passed
        if (_firetime >= shootCooldown) {
            fire();
            _firetime = 0.f;  // Reset the fire timer after shooting
        }
    }
}

void PlayerTurretComponent::fire() const {
    if (!_parent) {
        std::cerr << "Error: _parent is null in fire()" << std::endl;
        return;
    }
    if (!_parent->scene) {
        std::cerr << "Error: _parent->scene is null in fire()" << std::endl;
        return;
    }

    // Create the bullet entity
    auto bullet = _parent->scene->makeEntity();

    // Set the spawn position based on the player's facing direction
    sf::Vector2f spawnOffset = (isFacingLeft) ? sf::Vector2f(-10.f, 0.f) : sf::Vector2f(10.f, 0.f);
    bullet->setPosition(_parent->getPosition() + spawnOffset);  // Bullet starts at the player's position offset

    // Add components to the bullet (Shape, Bullet, Physics)
    bullet->addComponent<BulletComponent>();
    auto s = bullet->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(8.f);  // Bullet size
    s->getShape().setFillColor(sf::Color::Red);
    s->getShape().setOrigin(sf::Vector2f(8.f, 8.f)); // Center origin of the bullet

    // Add PhysicsComponent to the bullet
    auto p = bullet->addComponent<PhysicsComponent>(true, sf::Vector2f(8.f, 8.f));
    p->setRestitution(0.4f);
    p->setFriction(0.005f);

    // Bullet speed
    float bulletSpeed = 50.f;

    // Direction vector: Default is right (1.f in x)
    // Bullet direction depends on whether the player is facing left or right
    sf::Vector2f direction = (isFacingLeft) ? sf::Vector2f(-1.f, 0.f) : sf::Vector2f(1.f, 0.f);

    // Apply the velocity in the direction the player is facing
    sf::Vector2f velocity = direction * bulletSpeed;

    // Apply the velocity as an impulse to the bullet
    p->impulse(velocity);

    // Debugging output (optional)
    std::cout << "Fired bullet with velocity: " << velocity.x << ", " << velocity.y << std::endl;
}