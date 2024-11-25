#pragma once

#include "cmp_actor_movement.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
  sf::Vector2f _direction;

public:
  void update(double dt) override;

  explicit EnemyAIComponent(Entity* p);

  EnemyAIComponent() = delete;
};

class WheelsAIComponent : public EnemyAIComponent {

public:
  explicit WheelsAIComponent(Entity* p);

  void update(double dt) override;

  WheelsAIComponent() = delete;
}; 