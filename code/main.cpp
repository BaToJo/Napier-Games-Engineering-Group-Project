#include "../lib_engine/Engine.h"
#include "../lib_tile_level_loader/TileMap.h"
#include "game.h"

// DEBUG, DELETE THIS
#include "../lib_JSON_reader/TileMap_Importer.cpp"

using namespace std;

GameScene gameScene;

int main()
{
	//TileMap* tileMap = TileMap_Importer::LoadMap("C:\\Users\\Jonathan\\University\\GamesEng\\Napier-Games-Engineering-Group-Project\\res\\levels\\test_large_embeddedTileset.tmj");

	//std::cout << "tileMap->height = " << tileMap->height << std::endl;
	//std::cout << "tileMap->infinite = " << tileMap->infinite << std::endl;
	//std::cout << "tileMap->tile_layer.height = " << tileMap->tile_layer.height << std::endl;
	//std::cout << "tileMap->tile_layer.chunks[0].height = " << tileMap->tile_layer.chunks[0].height << std::endl;
	//std::cout << "tileMap->waypoint_layer.waypoints[0].x = " << tileMap->waypoint_layer.waypoints[0].x << std::endl;


	srand(time(0));
	Engine::Start(1280, 720, "Cartharsis", &gameScene);
	return 0;
}