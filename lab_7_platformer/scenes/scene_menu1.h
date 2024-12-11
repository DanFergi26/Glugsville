#pragma once

#include "engine.h"

class Menu1Scene : public Scene {
public:
	Menu1Scene() = default;
	~Menu1Scene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
