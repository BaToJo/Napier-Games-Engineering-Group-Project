#pragma once
#include "json.hpp"
#include "../lib_tile_level_loader/TileMap.h"
#include "SFML/Window/Keyboard.hpp"

using json = nlohmann::json;

namespace TileMap_Importer
{
	static std::string currentResolution;
	TileMap* LoadMap(const std::string fileName);
	sf::Texture* GetTileTexture(TileMap* tileMap, int id);
	bool GetTileSolid(TileMap* tileMap, int id);
	std::string GetTileAudioDriveOn(TileMap* tileMap, int id);
	std::vector<sf::Keyboard::Key> KeyboardControls_LoadFromFile(std::string file_name);
	void KeyboardControls_WriteToFile(std::string file_name, std::vector<sf::Keyboard::Key> controls);
	void Settings_WriteToFile(std::string file_name, bool vsync_enabled, bool fullscreen_enabled, bool backgroundMusic_enabled, bool soundEffects_enabled, std::string resolution);
	std::vector<std::pair<std::string, bool>> Settings_ReadFromFile(std::string file_name);

} // namespace TileMap_Importer