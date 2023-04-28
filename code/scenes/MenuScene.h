#pragma once 

#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include <SFML/Graphics.hpp>
class MenuScene : public Scene
{
private:

public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;
};