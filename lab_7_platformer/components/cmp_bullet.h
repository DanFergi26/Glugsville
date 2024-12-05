#pragma once
#include "cmp_physics.h"
#include <ecm.h>

/// BulletComponent handles the behavior of bullets in the game.
class BulletComponent : public Component {
protected:
    /// The lifetime of the bullet in seconds.
    float _lifetime;
    float isFacingLeft;

public:
    /// Updates the bullet component every frame.
    void update(double dt) override;

    /// Renders the bullet (currently does nothing).
    void render() override {}

    /// Constructor to initialize the bullet with a given lifetime.
    explicit BulletComponent(Entity* p, float lifetime = 3.f);

    /// Deleted default constructor.
    BulletComponent() = delete;

    /// Checks if the bullet has collided with a wall.
    /// @return True if a collision with a wall is detected; otherwise, false.
    bool checkWallCollision() const;


};
