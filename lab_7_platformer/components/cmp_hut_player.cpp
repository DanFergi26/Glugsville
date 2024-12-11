#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
    if (auto pl = _player.lock()) {
        // Get positions of the player and enemy
        auto playerPos = pl->getPosition();
        auto enemyPos = _parent->getPosition();

        // Horizontal and vertical distance
        float xDiff = abs(playerPos.x - enemyPos.x); // Horizontal difference
        float yDiff = playerPos.y - enemyPos.y;     // Vertical difference (player above if negative)

        // Check if the player is above and close enough
        if (xDiff < 25.0 && yDiff < -10.0 && yDiff > -50.0) { // Customize thresholds as needed
            _parent->setForDelete(); // Delete the enemy (player jumps on it)
        }
        // Check for general collision (not above)
        else if (xDiff < 25.0 && abs(yDiff) < 20.0) { // Adjust thresholds for horizontal collision
            pl->setForDelete(); // "Kill" the player
            // Optionally, you can add logic to decrement player lives instead of outright deletion
            // Example: pl->decrementLives();
        }
    }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}