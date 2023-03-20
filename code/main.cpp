#include "../lib_engine/Engine.h"
#include "game.h"

using namespace std;

GameScene gameScene;

int main()
{
    Engine::Start(1280, 720, "Cartharsis", &gameScene);
    return 0;
}