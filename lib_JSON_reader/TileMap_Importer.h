#pragma once
#include "json.hpp"
#include "../lib_tile_level_loader/TileMap.h"

using json = nlohmann::json;


namespace TileMap_Importer
{
	TileMap* LoadMap(const std::string fileName);
	sf::Texture* GetTileTexture(TileMap* tileMap, int id);

} // namespace TileMap_Importer