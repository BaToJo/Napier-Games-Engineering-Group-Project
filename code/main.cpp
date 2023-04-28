#include "../lib_engine/Engine.h"
#include "game.h"

using namespace std;

GameScene gameScene;
MenuScene menuScene;
int main()
{
    Engine::Start(1280, 720, "Cartharsis", &menuScene);
    return 0;
}