#include <SFML/Graphics.hpp>
#include "LevelSystem.h"
using namespace sf;
using namespace std;

View playerCamera;
void Load()
{
    ls::loadLevelFile("res/levels/pacman.txt", 50.f);
}

void Update()
{
    static Clock clock;
    float dt = clock.restart().asSeconds();
    playerCamera.move(Vector2f(dt * 20.f, 0));
}

void Render(RenderWindow& window)
{
    ls::Render(window);
}

int main()
{
    Load();
    RenderWindow window(VideoMode({800, 600}), "SFML works!");
    playerCamera.setCenter(Vector2f(200.f, 200.f));
    playerCamera.setSize(Vector2f(500.f, 500.f));
    window.setView(playerCamera);
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