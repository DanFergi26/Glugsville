#pragma once
#include <SFML/Audio.hpp>
#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

	sf::Music music;
};
