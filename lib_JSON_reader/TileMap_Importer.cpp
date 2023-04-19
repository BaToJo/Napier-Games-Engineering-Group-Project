#include "TileMap_Importer.h"
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

json game_map;

namespace TileMap_Importer
{
	//const int positionIterations = 2;


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


		// cout << allData;
		//auto at = allData.at("layers");
		//cout << at;
		//auto find = allData.find("layers");
		//auto find_at = find->at("height");
		//cout << "\n\n" + find_at;



		//cout << "\n\ncompressionlevel = " + allData["compressionlevel"];
		//cout << "\n\nheight = " + allData["height"];
		//cout << "\n\ninfinite = " + allData["infinite"];
		//cout << "\n\nlayers = " + allData["layers"];


		//// For each layers -> chunks...
		//for (const auto& chunk : allData["layers"]["chunks"])
		//{
		//	// If the chunk contains a data region...
		//	if (chunk.find("data") != chunk.end())
		//	{
		//		// For each 
		//		for (const auto& data : chunk["data"])
		//		{
		//			std::cout << data << std::endl;
		//		}
		//	}
		//}

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
			TileMap::layer layer_instance;

			layer_instance.height = layer["height"];
			layer_instance.id = layer["id"];
			layer_instance.name = layer["name"];
			layer_instance.opacity = layer["opacity"];
			layer_instance.startx = layer["startx"];
			layer_instance.starty = layer["starty"];
			layer_instance.type = layer["type"];
			layer_instance.visible = layer["visible"];
			layer_instance.width = layer["width"];
			layer_instance.x = layer["x"];
			layer_instance.y = layer["y"];

			for (const auto& chunk : layer["chunks"])
			{
				TileMap::chunk chunk_instance;

				for (const auto& element : chunk["data"])
				{
					chunk_instance.data.push_back(element);
				}

				chunk_instance.height = chunk["height"];
				chunk_instance.width = chunk["width"];
				chunk_instance.x = chunk["x"];
				chunk_instance.y = chunk["y"];

				layer_instance.chunks.push_back(chunk_instance);
			}

			tileMap->layers.push_back(layer_instance);
		}

		for (const auto& tileset : game_map["tilesets"])
		{
			TileMap::tileset tileset_instance;

			tileset_instance.columns = tileset["columns"];
			tileset_instance.firstgid = tileset["firstgid"];
			tileset_instance.margin = tileset["margin"];
			tileset_instance.name = tileset["name"];
			tileset_instance.spacing = tileset["spacing"];
			tileset_instance.tilecount = tileset["tilecount"];
			tileset_instance.tileheight = tileset["tileheight"];
			tileset_instance.tilewidth = tileset["tilewidth"];

			TileMap::grid grid_instance;

			grid_instance.height = tileset["grid"]["height"];
			grid_instance.orientation = tileset["grid"]["orientation"];
			grid_instance.width = tileset["grid"]["width"];

			tileset_instance.grid = grid_instance;

			TileMap::transformations transformations_instance;

			transformations_instance.hflip = tileset["transformations"]["hflip"];
			transformations_instance.preferuntransformed = tileset["transformations"]["preferuntransformed"];
			transformations_instance.rotate = tileset["transformations"]["rotate"];
			transformations_instance.vflip = tileset["transformations"]["vflip"];

			tileset_instance.transformations = transformations_instance;

			for (const auto& tile : tileset["tiles"])
			{
				TileMap::tile tile_instance;

				tile_instance.id = tile["id"];
				tile_instance.image = tile["image"];
				tile_instance.imageheight = tile["imageheight"];
				tile_instance.imagewidth = tile["imagewidth"];

				tileset_instance.tiles.push_back(tile_instance);
			}


		}


		// tileMap.tilesets

		//std::cout << "tileMap->height = " << tileMap->height << std::endl;
		//std::cout << "tileMap->infinite = " << tileMap->infinite << std::endl;
		//std::cout << "tileMap->layers[0].height = " << tileMap->layers[0].height << std::endl;
		//std::cout << "tileMap->layers[0].chunks[0].height = " << tileMap->layers[0].chunks[0].height << std::endl;

		//for (const auto& map_layer : game_map["layers"])
		//{
		//	std::cout << map_layer["height"] << std::endl;
		//	//std::cout << layers["height"] << std::endl;
		//}

		return tileMap;

		//for (const auto& item : game_map["items"]["item"])
		//{
		//	std::cout << item["ppu"] << std::endl;


		//	// check if item["batters"]["batter"] exists

		//	if (item.find("batters") != item.end() and
		//		item["batters"].find("batter") != item["batters"].end())
		//	{
		//		for (const auto& batter : item["batters"]["batter"])
		//		{
		//			std::cout << batter["id"] << ", " << batter["type"] << std::endl;
		//		}
		//	}

		//	// check if item["topping"] exists
		//	if (item.find("topping") != item.end())
		//	{
		//		for (const auto& batter : item["topping"])
		//		{
		//			std::cout << batter["id"] << ", " << batter["type"] << std::endl;
		//		}
		//	}
		//}
	}



} // namespace Tilemap_Importer













