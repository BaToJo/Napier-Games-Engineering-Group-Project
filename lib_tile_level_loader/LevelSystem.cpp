
#include "LevelSystem.h"
#include <fstream>
#include "TileMap.h"
#include "../lib_JSON_reader/TileMap_Importer.h"
#include "../lib_engine/Engine.h"
#include "../code/components/Cmp_Waypoint.h"
// #include "../code/components/Cmp_Sprite.h"

using namespace std;
using namespace sf;

TileMap tileMap;

std::map<int, std::shared_ptr<Entity>> waypoint_lookup_table; // Matches a waypoint's ID to its pointer.

std::map<int, std::shared_ptr<Entity>> LevelSystem::GetWaypoints()
{
	return waypoint_lookup_table;
}

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours
{
	{WALL, Color::White},
	{END, Color::Red}
};

sf::Color LevelSystem::getColor(LevelSystem::Tile t)
{
	auto it = _colours.find(t);
	if (it == _colours.end()) {
		_colours[t] = Color::Transparent;
	}
	return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c)
{
	_colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
// Vector2f LevelSystem::_offset(0,0);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::LoadWaypoints(Scene* scene, float tileSize)
{
	// Create every waypoint first with no destinations, to allow us to refer them to each other afterwards.
	for (auto& waypoint_data : tileMap.waypoint_layer.waypoints)
	{

		int tileMap_tile_width = tileMap.tilesets[0].tilewidth;
		int tileMap_tile_height = tileMap.tilesets[0].tileheight;

		// Create a new entity.
		std::shared_ptr<Entity> waypoint = scene->MakeEntity();

		int tile_position_x = ((waypoint_data.x / tileMap_tile_width) + 0.5) * tileSize;
		int tile_position_y = ((waypoint_data.y / tileMap_tile_height) + 0.5) * tileSize;

		// Get its ID and radius.
		int id = waypoint_data.id;
		float trigger_radius = (((waypoint_data.height / tileMap_tile_height) + (waypoint_data.width / tileMap_tile_width)) / 4) * tileSize; // Currently we assume all triggers are circles. If a waypoint is put in Tiled as elliptical, this will take its radius as the average of its width and height.


		// Position the waypoint in the correct place.
		waypoint->setPosition(sf::Vector2f(tile_position_x, tile_position_y));

		// TODO: Not yet implemented in Tiled. Waypoints in Tiled do not yet contain this information. When implemented, this boolean should come from the waypoint data imported from the JSON file.
		bool allow_multiple_vehicles_to_come_here_simultaneously = true;

		// Give our new waypoint its Waypoint component.
		waypoint->addComponent<AIWaypointComponent>(id, trigger_radius, allow_multiple_vehicles_to_come_here_simultaneously);

		// Add our new waypoint to the lookup table so we can access it quickly later from its ID.
		waypoint_lookup_table.insert(pair<int, std::shared_ptr<Entity>>(id, waypoint));


		// For debugging purposes, give every waypoint a green circle representing its radius.
		//auto shapeComponent = waypoint->addComponent<ShapeComponent>();
		//sf::Vector2f size = Vector2f(trigger_radius * 2, trigger_radius * 2);
		//shapeComponent->setShape<sf::CircleShape>(size);
		//shapeComponent->getShape().setFillColor(Color::Green);
		//shapeComponent->getShape().setOrigin(Vector2f(trigger_radius, trigger_radius));

	}

	// Give each waypoint its correct destinations.
	std::vector<std::shared_ptr<Entity>> destinations;
	// For each waypoint...
	for (auto& waypoint_data : tileMap.waypoint_layer.waypoints)
	{
		// For each destination...
		for (auto& waypoint_data_destination : waypoint_data.properties)
		{
			// Get a pointer to this destination.
			int destination_id = waypoint_data_destination.value;
			shared_ptr<Entity> destination = waypoint_lookup_table.at(destination_id);

			// Add this destination to this waypoint.
			waypoint_lookup_table.at(waypoint_data.id)->getComponents<AIWaypointComponent>()[0]->AddDestination(destination);
		}
	}

}

void LevelSystem::LoadTileMap(float tileSize)
{
	// The tilemap divides up its data into chunks. We have to iterate over them.
	for (auto& chunk : tileMap.tile_layer.chunks)
	{
		// Each chunk stores its data as an array of integers, where each integer is the ID for the type of tile at that position.
		// We have to go through this array of integers and spawn the correct type of tile at each position.
		for (int i = 0; i < chunk.data.size(); ++i)
		{
			uint32_t tile_ID = chunk.data[i];

			// A zero means no tile should be placed at this coordinate, so we move on to the next tile instead.
			if (tile_ID == 0) continue;


			// Tiled stores information about rotations and mirroring using the 3 most significant bits of the unsigned 32-bit int that it's stored in.
			bool mirrored = (tile_ID >> 31) & 1;
			bool flipped = (tile_ID >> 30) & 1;
			bool rotated_clockwise_once = (tile_ID >> 29) & 1;

			// Set those bits to zero so we can compare the integer to things as a decimal later.
			uint32_t tile_type = tile_ID;
			tile_type &= ~(1 << 31);
			tile_type &= ~(1 << 30);
			tile_type &= ~(1 << 29);


			// The chunk has metadata showing its width and height, so we know how to wrap the array into a rectangle shape.
			int tile_x = chunk.x + (i % chunk.width);
			int tile_y = chunk.y + (i / chunk.width);

			// Spawn a tile.
			auto tile = make_unique<sf::RectangleShape>();
			// Set its position and dimensions.
			tile->setOrigin(Vector2f(tileSize / 2, tileSize / 2));

			tile->setPosition(sf::Vector2f(tile_x * tileSize, tile_y * tileSize));
			tile->setSize(sf::Vector2f(tileSize, tileSize));
			if (mirrored)
			{
				tile->scale(sf::Vector2f(-1, 1));
			}
			if (flipped)
			{
				tile->scale(sf::Vector2f(1, -1));
			}
			if (rotated_clockwise_once)
			{
				tile->setRotation(sf::degrees(90));
			}

			// You can give the tile a solid color for debugging purposes.
			//tile->setFillColor(Color::Red);
			//tile->setFillColor(t.c);
			//tile->setFillColor(Color(rand()%255,rand()%255,rand()%255));

			// Ask the tileset to serve us the texture tied to this tile's ID.
			sf::Texture* texture = TileMap_Importer::GetTileTexture(&tileMap, tile_type);
			if (texture != nullptr)
			{
				tile->setTexture(texture);
			}
			else
			{
				// For debugging, we colour red the tiles whose texture the tileset can't find.
				tile->setFillColor(Color::Red);
			}

			_sprites.push_back(move(tile));
		}
	}

}

void LevelSystem::LoadLevelFile(const std::string& path, Scene* scene, float tileSize)
{
	TileMap* tileMap_pointer = TileMap_Importer::LoadMap(path);
	tileMap = *tileMap_pointer;
	// We allocated heap memory for it with "new", so now that we've assigned it by value to our stack variable, we should free the heap memory. Yes, this is a bit of a waste of memory moving but we only do this once at the start. We want this LevelSystem to be the owner of the data.
	delete(tileMap_pointer);

	LoadTileMap(tileSize);
	LoadWaypoints(scene, tileSize);

}

void LevelSystem::LoadLevelFile_OLD(const std::string& path, float tileSize)
{
	_tileSize = tileSize;
	size_t w = 0, h = 0;
	string buffer;

	// Load in file to buffer
	ifstream f(path);
	if (f.good())
	{
		f.seekg(0, std::ios::end);
		buffer.resize(f.tellg());
		f.seekg(0);
		f.read(&buffer[0], buffer.size());
		f.close();
	}
	else
	{
		throw string("Couldn't open level file: ") + path;
	}

	std::vector<Tile> temp_tiles;
	int widthCheck = 0;
	for (int i = 0; i < buffer.size(); ++i)
	{
		const char c = buffer[i];
		if (c == '\0')
		{
			break;
		}
		if (c == '\n')
		{ // newline
			if (w == 0)
			{  // if we haven't written width yet
				w = i;       // set width
			}
			else if (w != (widthCheck - 1))
			{
				throw string("non uniform width:" + to_string(h) + " ") + path;
			}
			widthCheck = 0;
			h++; // increment height
		}
		else
		{
			temp_tiles.push_back((Tile)c);
		}
		++widthCheck;
	}

	if (temp_tiles.size() != (w * h))
	{
		throw string("Can't parse level file") + path;
	}
	_tiles = std::make_unique<Tile[]>(w * h);
	_width = w; // set static class vars
	_height = h;
	std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
	cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
	BuildSprites();
}

void LevelSystem::BuildSprites(bool optimise)
{
	_sprites.clear();

	struct tp
	{
		sf::Vector2f p;
		sf::Vector2f s;
		sf::Color c;
	};
	vector<tp> tps;
	const auto tls = Vector2f(_tileSize, _tileSize);
	for (size_t y = 0; y < _height; ++y)
	{
		for (size_t x = 0; x < _width; ++x)
		{
			Tile t = getTile({ x, y });
			if (t == EMPTY)
			{
				continue;
			}
			tps.push_back({ getTilePosition({x, y}), tls, getColor(t) });
		}
	}

	const auto nonempty = tps.size();

	// If tile of the same type are next to each other,
	// We can use one large sprite instead of two.
	if (optimise && nonempty)
	{

		vector<tp> tpo;
		tp last = tps[0];
		size_t samecount = 0;

		for (size_t i = 1; i < nonempty; ++i)
		{
			// Is this tile compressible with the last?
			bool same = ((tps[i].p.y == last.p.y) &&
				(tps[i].p.x == last.p.x + (tls.x * (1 + samecount))) &&
				(tps[i].c == last.c));
			if (same)
			{
				++samecount; // Yes, keep going
				// tps[i].c = Color::Green;
			}
			else
			{
				if (samecount)
				{
					last.s.x = (1 + samecount) * tls.x; // Expand tile
				}
				// write tile to list
				tpo.push_back(last);
				samecount = 0;
				last = tps[i];
			}
		}
		// catch the last tile
		if (samecount)
		{
			last.s.x = (1 + samecount) * tls.x;
			tpo.push_back(last);
		}

		// No scan down Y, using different algo now that compressible blocks may
		// not be contiguous
		const auto xsave = tpo.size();
		samecount = 0;
		vector<tp> tpox;
		for (size_t i = 0; i < tpo.size(); ++i) {
			last = tpo[i];
			for (size_t j = i + 1; j < tpo.size(); ++j)
			{
				bool same = ((tpo[j].p.x == last.p.x) && (tpo[j].s == last.s) &&
					(tpo[j].p.y == last.p.y + (tls.y * (1 + samecount))) &&
					(tpo[j].c == last.c));
				if (same)
				{
					++samecount;
					tpo.erase(tpo.begin() + j);
					--j;
				}
			}
			if (samecount)
			{
				last.s.y = (1 + samecount) * tls.y; // Expand tile
			}
			// write tile to list
			tpox.push_back(last);
			samecount = 0;
		}

		tps.swap(tpox);
	}

	for (auto& t : tps)
	{
		auto s = make_unique<sf::RectangleShape>();
		s->setPosition(t.p);
		s->setSize(t.s);
		s->setFillColor(Color::Red);
		s->setFillColor(t.c);
		// s->setFillColor(Color(rand()%255,rand()%255,rand()%255));
		_sprites.push_back(move(s));
	}

	cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty
		<< " Not Empty, using: " << _sprites.size() << " Sprites\n";
}

void LevelSystem::Render(RenderWindow& window)
{
	for (auto& t : _sprites) {
		window.draw(*t);
	}
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p)
{
	if (p.x > _width || p.y > _height) {
		throw string("Tile out of range: ") + to_string(p.x) + "," +
			to_string(p.y) + ")";
	}
	return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p)
{
	return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type)
{
	auto v = vector<sf::Vector2ul>();
	for (size_t i = 0; i < _width * _height; ++i) {
		if (_tiles[i] == type) {
			v.push_back({ i % _width, i / _width });
		}
	}

	return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v)
{
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0) {
		throw string("Tile out of range ");
	}
	return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v)
{
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0) {
		return false;
	}
	auto p = Vector2ul((v - _offset) / (_tileSize));
	if (p.x > _width || p.y > _height) {
		return false;
	}
	return true;
}

void LevelSystem::setOffset(const Vector2f& _offset)
{
	LevelSystem::_offset = _offset;
	BuildSprites();
}

void LevelSystem::Unload()
{
	cout << "LevelSystem unloading\n";
	_sprites.clear();
	_tiles.reset();
	_width = 0;
	_height = 0;
	_offset = { 0, 0 };
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }