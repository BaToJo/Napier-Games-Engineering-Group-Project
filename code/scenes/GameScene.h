#pragma once 

#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include "../ContactListener.h"

class GameScene : public Scene
{
private:
	ContactListener* _contactListener;
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;

};