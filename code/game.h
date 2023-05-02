#pragma once

#include "scenes/GameScene.h"
#include "scenes/MenuScene.h"
#include "scenes/SettingsScene.h"

extern GameScene gameScene;
extern MenuScene menuScene;
extern SettingsScene settingsScene;
extern std::map<std::string, bool> _settings;
