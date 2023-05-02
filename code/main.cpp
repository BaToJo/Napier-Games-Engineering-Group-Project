#include "../lib_engine/Engine.h"
#include "game.h"

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
    Engine::Start(1280, 720, "Cartharsis", &menuScene);
    return 0;
}