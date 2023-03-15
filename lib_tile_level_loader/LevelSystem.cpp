 //LevelSystem.cpp
#include "LevelSystem.h"
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 20.0f);

float LevelSystem::_previousScore;
float LevelSystem::_bestScore;
float LevelSystem::_tileSize(100.f);

vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours{ {WALL, Color::Blue }, {END, Color::Red} };

sf::Color LevelSystem::getColor(LevelSystem::TILE t) 
{
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::TILE t, sf::Color c) 
{
    auto it = _colours.find(t);
    if (it != _colours.end())
        it->second = c;
}

size_t LevelSystem::getWidth()
{
    return _width;
}

size_t LevelSystem::getHeight()
{
    return _height;
}

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) 
{
    loadScores();
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

    std::vector<TILE> temp_tiles;
    for (int i = 0; i < buffer.size(); ++i) 
    {
        const char c = buffer[i];
        switch (c) 
        {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 's':
            temp_tiles.push_back(START);
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case '+':
            temp_tiles.push_back(WAYPOINT);
            break;
        case 'n':
            temp_tiles.push_back(ENEMY);
            break;
        case '\n':      // end of line
            if (w == 0) 
            { // if we haven't written width yet
                w = i;      // set width
            }
            h++; // increment height
            break;
        default:
            cout << c << endl; // Don't know what this tile type is
        }
    }
    if (temp_tiles.size() != (w * h)) 
    {
        throw string("Can't parse level file") + path;
    }
    _tiles = std::make_unique<TILE[]>(w * h);
    _width = w; //set static class vars
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;

    buildSprites();
}

void LevelSystem::buildSprites()
{
    _sprites.clear();
    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) 
    {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) 
        {
            auto s = make_unique<RectangleShape>();

            
            s->setPosition(getTilePosition({ x, y}) + Vector2f(0.f, 20.f));
            s->setSize(Vector2f(_tileSize, _tileSize));
            s->setFillColor(getColor(getTile({ x, y })));

            _sprites.push_back(move(s));
        }
    }
}

void LevelSystem::loadScores()
{
    string filePath = "res/score/scores.txt";
    ifstream scoreStream;
    std::cout << std::filesystem::absolute(filePath) << endl;
    scoreStream.open(filePath);

    string line;
    int index = 0;
    while (getline(scoreStream, line))
    {
        string score = line.substr(1);
        if (index == 0)
        {
            _bestScore = stof(score);
        }
        else
        {
            _previousScore = stof(score);
        }

        index++;
    }

    
}

void LevelSystem::updateScores(float currentScore)
{
    fstream newScore;
    newScore.open("res/score/string_out.txt", ios::out);

    ifstream scoreStream;
    scoreStream.open("res/score/scores.txt");

    string currLine;
    int index = 0;

    while (getline(scoreStream, currLine))
    {
        if (index == 0)
        {
            string score = currLine.substr(1);
            float fScore = stof(score);

            if (fScore < currentScore && fScore != 0.f)
            {
                newScore << "B " << fScore << endl;
            }
            else
            {
                newScore << "B " << currentScore << endl;
            }
        }

        if (index == 1)
        {
            newScore << "P " << currentScore;
        }

        index++;
    }

    newScore.close();
    scoreStream.close();
    remove("res/score/scores.txt");
    rename("res/score/string_out.txt", "res/score/scores.txt");
}

Vector2f LevelSystem::getTilePosition(Vector2ul p) 
{
    return (Vector2f(p.x, p.y) * _tileSize);
}

LevelSystem::TILE LevelSystem::getTile(Vector2ul p) 
{
    if (p.x > _width || p.y > _height) 
    {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

LevelSystem::TILE LevelSystem::getTileAt(Vector2f v) 
{
    auto a = v - _offset;
    if (a.x < 0 || a.y < 0) 
    {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(TILE t)
{
    std::vector<sf::Vector2ul> vecOfTiles;
    for (int i = 0; i < _width * _height; i++)
    {
        if (_tiles[i] == t)
        {
            vecOfTiles.push_back({ i % _width, i / _width });
        }
    }
   
    return vecOfTiles;
}

sf::Vector2f LevelSystem::getCentreOffset()
{
    return Vector2f({_tileSize / 2.f, _tileSize / 2.f + 20.f});
}

float LevelSystem::getPreviousScore()
{
    return _previousScore;
}

float LevelSystem::getBestScore()
{
    return _bestScore;
}


void LevelSystem::Render(RenderWindow& window) 
{
    for (size_t i = 0; i < _width * _height; ++i) 
    {
        window.draw(*_sprites[i]);
    }
}