#pragma once

#include <SFML/Graphics.hpp>
#include "..\lib_maths\maths.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "../lib_ecm/Ecm.h"
#include "../lib/b2d/Box2D/Box2D/Box2D.h"

#define ls LevelSystem

class LevelSystem
{
public:
	static void LoadTileMap(float tileSize);
	static void LoadWaypoints(Scene* scene, float tileSize);
	static void LoadLevelFile(const std::string& path, Scene* scene_instance, float tileSize);
	static void LoadLevelFile_OLD(const std::string&, float tileSize = 100.0f);
	static void Unload();
	static void Render(sf::RenderWindow& window);

	typedef unsigned char Tile;

	enum TILES {
		EMPTY = ' ',
		START = 's',
		END = 'e',
		WALL = 'w',
		ENEMY = 'n',
		WAYPOINT = '+'
	};

	static std::map<int, std::shared_ptr<Entity>> GetWaypoints();

	static Tile getTile(sf::Vector2ul);

	static Tile getTileAt(sf::Vector2f);

	static bool isOnGrid(sf::Vector2f);

	static size_t getWidth();

	static size_t getHeight();

	static sf::Vector2f getTilePosition(sf::Vector2ul);

	static std::vector<sf::Vector2ul> findTiles(Tile);

	static sf::Color getColor(Tile t);

	static void setColor(Tile t, sf::Color c);

	static void setOffset(const sf::Vector2f& _offset);

	static const sf::Vector2f& getOffset();

	static float getTileSize();

protected:
	static std::unique_ptr<Tile[]> _tiles;
	static size_t _width;
	static size_t _height;
	static sf::Vector2f _offset;

	static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;
	static std::vector<b2Body*> _colliders_bodies;
	static std::vector<b2Fixture*> _colliders_fixtures;


	static void BuildSprites(bool optimise = true);
	static float _tileSize; // for rendering
	static std::map<Tile, sf::Color> _colours;

private:
	LevelSystem() = delete;

	~LevelSystem() = delete;
};