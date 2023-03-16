#include <SFML/Graphics.hpp>
#include "LevelSystem.h"
#include "Ecm.h"
#include "Cmp_player_movement.h"
#include "Cmp_Sprite.h"
#include "System_Renderer.h"
#include "Box2D/Box2D.h"

using namespace sf;
using namespace std;

shared_ptr<Entity> player;
View playerCamera;
EntityManager ents;
b2World* world;
void Load()
{
    ls::loadLevelFile("res/levels/pacman.txt", 50.f);

    player = make_shared<Entity>();
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(12.f);
    s->getShape().setFillColor(Color::Yellow);
    s->getShape().setOrigin(Vector2f(12.f, 12.f));

    player->addComponent<PlayerMovementComponent>();
    ents.list.push_back(player);
}

void Update()
{
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // The view's center is set to the player's position so the camera will center the player.
    Vector2f movement = player->getPosition() - playerCamera.getCenter();
    playerCamera.move(movement * dt * 5.f);

    

    ents.Update(dt);
}

void Render(RenderWindow& window)
{
    ls::Render(window);
    ents.Render();
    Renderer::Render();
}

int main()
{
    Load();
    RenderWindow window(VideoMode({800, 600}), "SFML works!");

    // View Setup
    playerCamera.setCenter(Vector2f(200.f, 200.f));
    playerCamera.setSize(Vector2f(500.f, 500.f));

    Renderer::Initialise(window);
        
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
        // View is set to the window after update (I assume you do camera calculations in the update)
        window.setView(playerCamera);
        Render(window);

        window.display();
    }
    return 0;
}