#include "../lib_engine/Engine.h"
#include "../lib_tile_level_loader/TileMap.h"
#include "game.h"

// DEBUG, DELETE THIS
#include "../lib_JSON_reader/TileMap_Importer.cpp"

using namespace std;

GameScene gameScene;
MenuScene menuScene;
SettingsScene settingsScene;
std::vector<std::pair<std::string, bool>> _settings{
	{"FULLSCREEN", false},
	{"VSYNC", false},
	{"BGM", true},
	{"EFFECTS", true},
	{"1920 x 1080", false},
	{"1280 x 720", true},
	{"800 x 600", false}
};

int main()
{
	// Seed the random number generator from the system time.
	srand(time(0));

	_settings = TileMap_Importer::Settings_ReadFromFile("res/player_settings/player_settings.json");

	sf::Vector2u resolution = sf::Vector2u(1280, 720);
	if (_settings[4].second)
		resolution = sf::Vector2u(1920, 1080);
	else if (_settings[5].second)
		resolution = sf::Vector2u(1280, 720);
	else if (_settings[6].second)
		resolution = sf::Vector2u(800, 600);
	Engine::Start(resolution.x, resolution.y, "Cartharsis", &menuScene, _settings[0].second);
	return 0;
}