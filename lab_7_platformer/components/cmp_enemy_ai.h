#pragma once

#include "cmp_actor_movement.h"

class EnemyAIComponent : public Component {
protected:
    float _speed;
    bool _flipSprite;  // New: Flag to enable sprite flipping based on direction

public:
    void update(double dt) override;
    void render() override {}

    // New: Method to enable sprite flipping
    void setFlipSpriteOnDirection(bool flip) { _flipSprite = flip; }

    explicit EnemyAIComponent(Entity* p);
    EnemyAIComponent() = delete;
};

