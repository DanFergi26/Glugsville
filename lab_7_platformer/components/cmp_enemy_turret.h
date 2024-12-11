#pragma once
#include "cmp_physics.h"
#include <ecm.h>

// RotatorComponent: Allows entities to spin
class RotatorComponent : public Component {
protected:
    float _rotationSpeed; // Rotation speed in degrees per second

public:
    explicit RotatorComponent(Entity* p, float speed = 0.f)
        : Component(p), _rotationSpeed(speed) {}

    void update(double dt) override {
        if (auto s = _parent->getComponent<SpriteComponent>()) {
            s->getSprite().rotate(_rotationSpeed * static_cast<float>(dt));
        }
    }

    void render() override {}

    void setRotationSpeed(float speed) { _rotationSpeed = speed; }
};

class EnemyTurretComponent : public Component {
protected:
    void fire() const;
    float _firetime;

public:
    void update(double dt) override;
    void render() override {}
    explicit EnemyTurretComponent(Entity* p);
    EnemyTurretComponent() = delete;
};
