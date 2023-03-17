#include "Cmp_player_movement.h"
#include "..\lib_tile_level_loader\LevelSystem.h"

const sf::Keyboard::Key controls[4] = 
{
    sf::Keyboard::W,
    sf::Keyboard::A,
    sf::Keyboard::S,
    sf::Keyboard::D
};

void PlayerMovementComponent::Update(double dt)
{
    if (sf::Keyboard::isKeyPressed(controls[0]))
    {
        Move(sf::Vector2f(0.0f, dt * -_speed));
    }
    if (sf::Keyboard::isKeyPressed(controls[1]))
    {
        Move(sf::Vector2f(dt * -_speed, 0.0f));
    }
    if (sf::Keyboard::isKeyPressed(controls[2]))
    {
        Move(sf::Vector2f(0.0f, dt * _speed ));
    }
    if (sf::Keyboard::isKeyPressed(controls[3]))
    {
        Move(sf::Vector2f( dt * _speed, 0.0f ));
    }

}
void PlayerMovementComponent::Render() {}
PlayerMovementComponent::PlayerMovementComponent(Entity* p) : ActorMovementComponent(p){}
