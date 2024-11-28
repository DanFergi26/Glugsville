#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerTurretComponent : public Component {
protected:
    void fire() const;

public:
    void update(double dt) override;
    void render() override {}
    explicit PlayerTurretComponent(Entity* p);
    PlayerTurretComponent() = delete;

private:
    float _firetime;       // Timer for shooting
    float shootCooldown;   // Cooldown for shooting
};