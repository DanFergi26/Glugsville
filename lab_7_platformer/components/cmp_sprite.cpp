#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;

void SpriteComponent::setTexture(std::shared_ptr<sf::Texture> tex) {
    _texture = tex;
    _sprite->setTexture(*_texture);  // Set the texture to the sprite.
}

SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) {
    // Sync the sprite's position and rotation with the entity's.
    _sprite->setPosition(_parent->getPosition());
    _sprite->setRotation(_parent->getRotation()); // Apply entity rotation
}

void SpriteComponent::render() {
    Renderer::queue(_sprite.get());  // Queue the sprite for rendering
}

void ShapeComponent::update(double dt) {
    _shape->setPosition(_parent->getPosition());
    _shape->setRotation(_parent->getRotation()); // Apply entity rotation
}

void ShapeComponent::render() {
    Renderer::queue(_shape.get());  // Queue the shape for rendering
}

sf::Shape& ShapeComponent::getShape() const {
    return *_shape;
}

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const {
    return *_sprite;
}
