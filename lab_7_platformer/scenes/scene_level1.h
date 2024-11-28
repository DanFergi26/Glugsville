#pragma once
#include "engine.h"
#include <SFML/Audio.hpp>       // For SFML audio


class Level1Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  private:
  sf::Music music_;


};
