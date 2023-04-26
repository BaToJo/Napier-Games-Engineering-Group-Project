#pragma once
#include "json.hpp"
#include "../lib_engine/Engine.h"
#include "../code/scenes/GameScene.h"

using json = nlohmann::json;


namespace SaveLoad_ReadWriter
{
	void LoadFromFile(const std::string fileName);
	void SaveToFile(const std::string fileName);
} // namespace SaveLoad_ReadWriter