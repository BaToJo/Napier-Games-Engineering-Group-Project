#include "../lib_engine/Engine.h"
#include "game.h"

using namespace std;

GameScene gameScene;
MenuScene menuScene;
SettingsScene settingsScene;
int main()
{
    Engine::Start(1280, 720, "Cartharsis", &menuScene);
    return 0;
}