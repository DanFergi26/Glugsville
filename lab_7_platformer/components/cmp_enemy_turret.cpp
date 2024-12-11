#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
    _firetime -= dt;
    if (_firetime <= 0.f) {
        fire();
        _firetime = 1.f;
    }
    static float angle = 0.f;
    angle += 1.f * dt;

    _parent->setRotation(180.f + sin(angle) * 45.f);
}


void EnemyTurretComponent::fire() const {
    auto bullet = _parent->scene->makeEntity();
    bullet->setPosition(_parent->getPosition());

    // Load the bullet sprite texture
    std::shared_ptr<sf::Texture> bulletTexture = std::make_shared<sf::Texture>();
    if (!bulletTexture->loadFromFile("res/img/bucky.png")) { // Replace "bullet.png" with your sprite filename
        std::cerr << "Error loading bullet sprite!" << std::endl;
        return; // Exit if the texture cannot be loaded
    }

    // Add a SpriteComponent to the bullet and set the texture
    auto sprite = bullet->addComponent<SpriteComponent>();
    sprite->setTexture(bulletTexture);

    // Scale the bullet sprite to a smaller size if needed
    sf::Vector2f targetSize(30.f, 30.f); // Desired size for the bullet
    sf::Vector2u textureSize = bulletTexture->getSize();
    sprite->getSprite().setScale(
        targetSize.x / textureSize.x,
        targetSize.y / textureSize.y
    );

    // Center the origin of the bullet sprite
    sprite->getSprite().setOrigin(
        textureSize.x / 2.f,
        textureSize.y / 2.f
    );

    // Add components for bullet behavior
    bullet->addComponent<HurtComponent>();
    bullet->addComponent<BulletComponent>();

    // Add PhysicsComponent for movement
    auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
    p->setRestitution(0.4f);
    p->setFriction(0.005f);

    // Apply an impulse to make the bullet move
    sf::Vector2f velocity = sf::rotate(Vector2f(0, 15.f), -_parent->getRotation());
    p->impulse(velocity);

    // Set up bullet spinning
    auto rot = bullet->addComponent<RotatorComponent>();
    rot->setRotationSpeed(180.f); // Rotate 180 degrees per second
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) {}
