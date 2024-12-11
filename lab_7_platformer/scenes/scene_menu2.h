#pragma once

#include "engine.h"

class Menu2Scene : public Scene {
public:
	Menu2Scene() = default;
	~Menu2Scene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
