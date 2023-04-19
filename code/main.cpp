#include "../lib_engine/Engine.h"
#include "../lib_tile_level_loader/TileMap.h"
#include "game.h"

// DEBUG, DELETE THIS
#include "../lib_JSON_reader/TileMap_Importer.cpp"

using namespace std;

GameScene gameScene;

int main()
{
	TileMap* tileMap = TileMap_Importer::LoadMap("C:\\Users\\Jonathan\\University\\GamesEng\\Napier-Games-Engineering-Group-Project\\res\\levels\\test_tile_reading_embeddedTileset.tmj");

	std::cout << "tileMap->height = " << tileMap->height << std::endl;
	std::cout << "tileMap->infinite = " << tileMap->infinite << std::endl;
	std::cout << "tileMap->layers[0].height = " << tileMap->layers[0].height << std::endl;
	std::cout << "tileMap->layers[0].chunks[0].height = " << tileMap->layers[0].chunks[0].height << std::endl;

	// Engine::Start(1280, 720, "Cartharsis", &gameScene);
	return 0;
}