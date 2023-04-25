#pragma once 

#include "..\lib_engine\Engine.h"

class GameScene : public Scene
{
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;

	static std::vector<std::shared_ptr<Entity>> GetAllActors();
};