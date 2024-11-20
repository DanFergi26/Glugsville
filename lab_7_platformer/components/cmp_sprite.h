#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

public:
  // Deleted default constructor to enforce dependency injection via entity.
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);

  void update(double dt) override;
  void render() override;

  // Get reference to the sprite (so it can be manipulated by other parts of the code).
  sf::Sprite& getSprite() const;

  // Corrected the method name to setTexture instead of setTexure
  void setTexture(std::shared_ptr<sf::Texture> tex);
};

class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;

public:
  // Deleted default constructor to enforce dependency injection via entity.
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;

  // Get reference to the shape (so it can be manipulated by other parts of the code).
  sf::Shape& getShape() const;

  // Generic template method to create different types of shapes (e.g., RectangleShape, CircleShape)
  template <typename T, typename... Targs> 
  void setShape(Targs... params) {
    _shape.reset(new T(params...));  // Create and reset the shape with given params.
  }
};