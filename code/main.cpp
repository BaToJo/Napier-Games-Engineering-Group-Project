#include <SFML/Graphics.hpp>
#include "LevelSystem.h"
using namespace sf;
using namespace std;

void Load()
{
    ls::loadLevelFile("res/levels/pacman.txt", 50.f);
}

void Update()
{

}

void Render(RenderWindow& window)
{
    ls::Render(window);
}

int main()
{
    Load();
    RenderWindow window(VideoMode({800, 600}), "SFML works!");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        window.clear();

        Update();

        Render(window);

        window.display();
    }
    return 0;
}