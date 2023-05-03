#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics/Texture.hpp"

class TileMap
{
protected:
public:

	struct Tile
	{
		int id = -1;
		std::string image = "";
		int imageheight = -1;
		int imagewidth = -1;
		sf::Texture texture;
		std::string audio_on_drive = "";
		bool solid = false;
	};

	struct Grid
	{
		int height = -1;
		std::string orientation = "";
		int width = -1;
	};

	struct Transformations
	{
		bool hflip = true;
		bool preferuntransformed = false;
		bool rotate = true;
		bool vflip = true;
	};

	struct Tileset {             // Structure declaration
		int columns = -1;
		int firstgid = -1;
		Grid grid;
		int margin = -1;
		std::string name = "";
		int spacing = -1;
		int tilecount = -1;
		int tileheight = -1;
		std::vector<Tile> tiles;
		int tilewidth = -1;
		Transformations transformations;
	};       // Structure variable

	struct Chunk {             // Structure declaration
		std::vector<int> data;
		int height = -1;
		int width = -1;
		int x = -1;
		int y = -1;
	};       // Structure variable

	struct Tile_layer_struct {             // Structure declaration
		std::vector<Chunk> chunks;
		int height = -1;
		int id = -1;
		std::string name = "";
		int opacity = -1;
		int startx = -1;
		int starty = -1;
		std::string type = "";
		bool visible = true;
		int width = -1;
		int x = -1;
		int y = -1;
	};       // Structure variable

	struct Waypoint_property_group
	{
		std::string name = "";
		std::string type = "";
		int value = -1;
	};

	struct Waypoint
	{
		bool ellipse = true;
		int height = -1;
		int id = -1;
		std::string name = "";
		std::vector<Waypoint_property_group> properties;
		int rotation = -1;
		std::string type = "";
		bool visible = true;
		int width = -1;
		int x = -1;
		int y = -1;
	};

	struct Waypoint_layer_struct {             // Structure declaration
		std::string draworder = "";
		int id = -1;
		std::string name = "";
		std::vector<Waypoint> waypoints;
		int opacity = -1;
		std::string type = "";
		bool visible = true;
		int x = -1;
		int y = -1;
	};       // Structure variable


	int height = -1;
	bool infinite = true;
	Tile_layer_struct tile_layer;
	Waypoint_layer_struct waypoint_layer;
	int compressionlevel = -1;
	int nextlayerid = -1;
	int nextobjectid = -1;
	std::string orientation = "";
	std::string renderorder = "";
	std::string tiledversion = "";
	int tileheight = -1;

	std::vector<Tileset> tilesets;
	int tilewidth = -1;
	std::string type = "";
	std::string version = "";
	int width = -1;
};
