#pragma once
#include <string>
#include <vector>


class TileMap
{
protected:
public:

	struct tile
	{
		int id = -1;
		std::string image = "";
		int imageheight = -1;
		int imagewidth = -1;
	};

	struct grid
	{
		int height = -1;
		std::string orientation = "";
		int width = -1;
	};

	struct transformations
	{
		bool hflip = true;
		bool preferuntransformed = false;
		bool rotate = true;
		bool vflip = true;
	};

	struct tileset {             // Structure declaration
		int columns = -1;
		int firstgid = -1;
		grid grid;
		int margin = -1;
		std::string name = "";
		int spacing = -1;
		int tilecount = -1;
		int tileheight = -1;
		std::vector<tile> tiles;
		int tilewidth = -1;
		transformations transformations;
	};       // Structure variable

	struct chunk {             // Structure declaration
		std::vector<int> data;
		int height = -1;
		int width = -1;
		int x = -1;
		int y = -1;
	};       // Structure variable

	struct layer {             // Structure declaration
		std::vector<chunk> chunks;
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


	int height = -1;
	bool infinite = true;
	std::vector<layer> layers;

	int compressionlevel = -1;
	int nextlayerid = -1;
	int nextobjectid = -1;
	std::string orientation = "";
	std::string renderorder = "";
	std::string tiledversion = "";
	int tileheight = -1;

	std::vector<tileset> tilesets;
	int tilewidth = -1;
	std::string type = "";
	std::string version = "";
	int width = -1;
};