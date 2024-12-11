#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include <SFML/Audio.hpp>  // Include SFML Audio for sound functionality

class HurtComponent : public Component {
protected:
	void fire() const;
	std::weak_ptr<Entity> _player;

	// Add a static sound buffer and sound to play the hurt sound effect
	static sf::SoundBuffer hurtBuffer;  // Static buffer to load the sound once
	static sf::Sound hurtSound;         // Sound object to play the sound

public:
	void update(double dt) override;
	void render() override {}

	// Function to play the hurt sound
	void playHurtSound();
	void playEnemySound();

	explicit HurtComponent(Entity* p);
	HurtComponent() = delete;
};
