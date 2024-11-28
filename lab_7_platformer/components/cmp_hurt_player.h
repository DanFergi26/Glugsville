#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtComponent : public Component {
protected:
  void fire() const;
  std::weak_ptr<Entity> _player;
  int lives_; // lives variable for player

public:
  void update(double dt) override;
  void render() override {}
  explicit HurtComponent(Entity* p);
  HurtComponent() = delete;
  void setLives(int lives) { lives_ = lives;} // setlives function
};
