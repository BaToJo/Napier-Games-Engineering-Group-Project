#include "TileMap_Importer.h"
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

json game_map;

namespace TileMap_Importer
{
	/*
	 * This in its current state takes JSON files exported by the software called Tiled. https://www.mapeditor.org/
	 * You MUST set the options in Tiled as:
	 *
	 *		Edit -> Preferences... -> General -> Export Options -> Embed Tilesets = TRUE
	 *
	 *
	 * If you need this C++ class to interpret the JSON input differently...
	 *
	 *		Use this website: https://jsonviewer.stack.hu/
	 *
	 *		Load in the JSON data.
	 *
	 *		Anything that the website's viewer displays as square brackets [] is a list that must be iterated over.
	 *		Anything in curly brackets {} you can think of like a struct.
	 */
	TileMap* LoadMap(const std::string fileName)
	{
		std::ifstream f(fileName);
		game_map = json::parse(f);

		TileMap* tileMap = new TileMap();

		tileMap->compressionlevel = game_map["compressionlevel"];
		tileMap->height = game_map["height"];
		tileMap->infinite = game_map["infinite"];
		tileMap->nextlayerid = game_map["nextlayerid"];
		tileMap->nextobjectid = game_map["nextobjectid"];
		tileMap->orientation = game_map["orientation"];
		tileMap->renderorder = game_map["renderorder"];
		tileMap->tiledversion = game_map["tiledversion"];
		tileMap->tileheight = game_map["tileheight"];
		tileMap->tilewidth = game_map["tilewidth"];
		tileMap->type = game_map["type"];
		tileMap->version = game_map["version"];
		tileMap->width = game_map["width"];

		// tileMap.layers
		for (const auto& layer : game_map["layers"])
		{
			string layer_type = layer["type"];

			if (layer_type == "tilelayer")
			{
				// TileMap::tile_layer tile_layer_instance;
				tileMap->tile_layer.type = layer_type;
				tileMap->tile_layer.height = layer["height"];
				tileMap->tile_layer.id = layer["id"];
				tileMap->tile_layer.name = layer["name"];
				tileMap->tile_layer.opacity = layer["opacity"];
				tileMap->tile_layer.startx = layer["startx"];
				tileMap->tile_layer.starty = layer["starty"];
				tileMap->tile_layer.visible = layer["visible"];
				tileMap->tile_layer.width = layer["width"];
				tileMap->tile_layer.x = layer["x"];
				tileMap->tile_layer.y = layer["y"];

				for (const auto& chunk : layer["chunks"])
				{
					TileMap::Chunk chunk_instance;

					for (const auto& element : chunk["data"])
					{
						chunk_instance.data.push_back(element);
					}

					chunk_instance.height = chunk["height"];
					chunk_instance.width = chunk["width"];
					chunk_instance.x = chunk["x"];
					chunk_instance.y = chunk["y"];

					tileMap->tile_layer.chunks.push_back(chunk_instance);
				}

				// tileMap->layers.push_back(tile_layer_instance);

			}
			else if (layer_type == "objectgroup")
			{
				// TileMap::waypoint_layer waypoint_layer_instance;

				tileMap->waypoint_layer.draworder = layer["draworder"];
				tileMap->waypoint_layer.id = layer["id"];
				tileMap->waypoint_layer.name = layer["name"];
				tileMap->waypoint_layer.opacity = layer["opacity"];
				tileMap->waypoint_layer.type = layer["type"];
				tileMap->waypoint_layer.visible = layer["visible"];
				tileMap->waypoint_layer.x = layer["x"];
				tileMap->waypoint_layer.y = layer["y"];

				for (const auto& waypoint : layer["objects"])
				{
					TileMap::Waypoint waypoint_instance;

					waypoint_instance.ellipse = waypoint["ellipse"];
					waypoint_instance.height = waypoint["height"];
					waypoint_instance.id = waypoint["id"];
					waypoint_instance.name = waypoint["name"];
					waypoint_instance.rotation = waypoint["rotation"];
					waypoint_instance.type = waypoint["type"];
					waypoint_instance.visible = waypoint["visible"];
					waypoint_instance.width = waypoint["width"];
					waypoint_instance.x = waypoint["x"];
					waypoint_instance.y = waypoint["y"];

					for (const auto& waypoint_property_group : waypoint["properties"])
					{
						TileMap::Waypoint_property_group waypoint_property_group_instance;

						waypoint_property_group_instance.name = waypoint_property_group["name"];
						waypoint_property_group_instance.type = waypoint_property_group["type"];
						waypoint_property_group_instance.value = waypoint_property_group["value"];

						waypoint_instance.properties.push_back(waypoint_property_group_instance);
					}
					tileMap->waypoint_layer.waypoints.push_back(waypoint_instance);
				}
			}
		}

		for (const auto& tileset : game_map["tilesets"])
		{
			TileMap::Tileset tileset_instance;

			tileset_instance.columns = tileset["columns"];
			tileset_instance.firstgid = tileset["firstgid"];
			tileset_instance.margin = tileset["margin"];
			tileset_instance.name = tileset["name"];
			tileset_instance.spacing = tileset["spacing"];
			tileset_instance.tilecount = tileset["tilecount"];
			tileset_instance.tileheight = tileset["tileheight"];
			tileset_instance.tilewidth = tileset["tilewidth"];

			TileMap::Grid grid_instance;

			grid_instance.height = tileset["grid"]["height"];
			grid_instance.orientation = tileset["grid"]["orientation"];
			grid_instance.width = tileset["grid"]["width"];

			tileset_instance.grid = grid_instance;

			TileMap::Transformations transformations_instance;

			transformations_instance.hflip = tileset["transformations"]["hflip"];
			transformations_instance.preferuntransformed = tileset["transformations"]["preferuntransformed"];
			transformations_instance.rotate = tileset["transformations"]["rotate"];
			transformations_instance.vflip = tileset["transformations"]["vflip"];

			tileset_instance.transformations = transformations_instance;

			for (const auto& tile : tileset["tiles"])
			{
				TileMap::Tile tile_instance;

				tile_instance.id = tile["id"];
				tile_instance.image = tile["image"];
				tile_instance.imageheight = tile["imageheight"];
				tile_instance.imagewidth = tile["imagewidth"];

				sf::Texture tile_texture = sf::Texture();
				tile_texture.loadFromFile(tile_instance.image);
				tile_instance.texture = tile_texture;

				tileset_instance.tiles.push_back(tile_instance);
			}

			tileMap->tilesets.push_back(tileset_instance);
		}

		return tileMap;
	}

	/*
	 * Where this function asks for ID, that means the integer inside the data array of a map chunk.
	 * The ID identifies what type of tile should be spawned, so we can ask for the corresponding texture.
	 */
	sf::Texture* GetTileTexture(TileMap* tileMap, int id)
	{
		if (tileMap->tilesets.size() <= 0)
		{
			throw std::logic_error("Tried to get a texture from a tileMap, but the tileMap didn't have a tileset!\n\nHas the tileMap been initialized correctly? Has the tileMap been lost after being created (eg. garbage collected when it goes out of scope)?");
		}
		for (TileMap::Tile& tile : tileMap->tilesets[0].tiles)
		{
			if (tile.id == id - 1)
			{
				return &tile.texture;
			}
		}
		return nullptr;
	}

} // namespace Tilemap_Importer













