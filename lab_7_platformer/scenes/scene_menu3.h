#pragma once

#include "engine.h"

class Menu3Scene : public Scene {
public:
	Menu3Scene() = default;
	~Menu3Scene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
