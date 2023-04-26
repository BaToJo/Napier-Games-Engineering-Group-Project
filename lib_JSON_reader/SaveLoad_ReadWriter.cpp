#include "SaveLoad_ReadWriter.h"
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

json saveFile;

namespace SaveLoad_ReadWriter
{
	/*
	 * If you need this C++ class to interpret the JSON input differently...
	 *
	 *		Use this website: https://jsonviewer.stack.hu/
	 *
	 *		Load in the JSON data.
	 *
	 *		Anything that the website's viewer displays as square brackets [] is a list that must be iterated over.
	 *		Anything in curly brackets {} you can think of like a struct.
	 */
	void SaveToFile(const std::string fileName, Scene* scene)
	{
		for (auto& entity : scene->ents.list)
		{

		}


		std::ofstream f(fileName);
		f << std::setw(4) << saveFile << std::endl;
		f.close();
	}
	void LoadFromFile(const std::string fileName)
	{
		std::ifstream f(fileName);
		saveFile = json::parse(f);
		f.close();
	}
} // namespace SaveLoad_ReadWriter













