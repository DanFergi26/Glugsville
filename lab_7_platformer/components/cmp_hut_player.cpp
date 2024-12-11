#include "cmp_hurt_player.h"
#include "../scenes/scene_level1.h"
#include <SFML/Audio.hpp>
#include <iostream>

sf::SoundBuffer HurtComponent::hurtBuffer;  // Static sound buffer
sf::Sound HurtComponent::hurtSound;         // Static sound object

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
            _parent->setForDelete();
            playEnemySound();
        }
        // Check for general collision (not above)
        else if (xDiff < 25.0 && abs(yDiff) < 20.0) { // Adjust thresholds for horizontal collision
            pl->setForDelete(); // "Kill" the player
            // Play the hurt sound
            playHurtSound();
            // Optionally, you can add logic to decrement player lives instead of outright deletion
            // Example: pl->decrementLives();
        }
    }
}

void HurtComponent::playHurtSound() {
    // Load the sound buffer if it has not been loaded yet
    if (hurtBuffer.getSampleCount() == 0) {
        if (!hurtBuffer.loadFromFile("res/music/hurt_sound.wav")) {  // Ensure this path is correct
            std::cerr << "Failed to load hurt sound!" << std::endl;
        }
        else {
            hurtSound.setBuffer(hurtBuffer);
            std::cout << "Hurt sound loaded successfully!" << std::endl;
        }
    }

    // Play the sound
    if (hurtBuffer.getSampleCount() > 0) {  // Only play sound if loaded successfully
        hurtSound.play();
    }
}


void HurtComponent::playEnemySound() {
    // Load the sound buffer if it has not been loaded yet
    if (hurtBuffer.getSampleCount() == 0) {
        if (!hurtBuffer.loadFromFile("res/music/enemy-death.wav")) {  // Ensure this path is correct
            std::cerr << "Failed to load hurt sound!" << std::endl;
        }
        else {
            hurtSound.setBuffer(hurtBuffer);
            std::cout << "Hurt sound loaded successfully!" << std::endl;
        }
    }

    // Play the sound
    if (hurtBuffer.getSampleCount() > 0) {  // Only play sound if loaded successfully
        hurtSound.play();
    }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}