#pragma once
#include "json.hpp"
#include "../lib_tile_level_loader/TileMap.h"

using json = nlohmann::json;


namespace TileMap_Importer
{
	TileMap* LoadMap(const std::string fileName);

} // namespace Physics