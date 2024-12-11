#pragma once

#include "engine.h"

class Menu4Scene : public Scene {
public:
	Menu4Scene() = default;
	~Menu4Scene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
