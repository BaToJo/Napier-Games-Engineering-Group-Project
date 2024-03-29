#pragma once

#include "scenes/GameScene.h"
#include "scenes/MenuScene.h"
#include "scenes/SettingsScene.h"

extern GameScene gameScene;
extern MenuScene menuScene;
extern SettingsScene settingsScene;

extern std::vector < std::pair<std::string, bool>> _settings;
