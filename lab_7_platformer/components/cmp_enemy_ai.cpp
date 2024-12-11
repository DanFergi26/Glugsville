#include "cmp_enemy_ai.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <SFML/Graphics.hpp>
using namespace sf;

EnemyAIComponent::EnemyAIComponent(Entity* p)
    : Component(p), _speed(50.f), _flipSprite(false) {}

void EnemyAIComponent::update(double dt) {
    auto pos = _parent->getPosition();
    auto vel = Vector2f(_speed * dt, 0.f);

    // Reverse direction if hitting boundaries or obstacles
    if (ls::getTileAt(pos + Vector2f(20.f, 0)) == ls::WALL ||
        ls::getTileAt(pos - Vector2f(20.f, 0)) == ls::WALL) {
        _speed = -_speed; // Reverse direction
        vel = Vector2f(_speed * dt, 0.f);
    }

    // Move the enemy (directly modify its position)
    pos += vel;
    _parent->setPosition(pos); // Assuming setPosition exists on Entity

    // Flip the sprite if enabled
    if (_flipSprite) {
        auto spriteComp = _parent->getComponent<SpriteComponent>();
        if (spriteComp) {
            auto& sprite = spriteComp->getSprite();
            float scaleX = (_speed < 0) ? -1.f : 1.f; // Flip based on direction
            sprite.setScale(abs(sprite.getScale().x) * scaleX, sprite.getScale().y);
        }
    }
}
