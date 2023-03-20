#include "Cmp_Player_Physics.h"
#include "..\lib_engine\System_Physics.h"
#include <SFML/Window/Keyboard.hpp>

using namespace Physics;

void PlayerPhysicsComponent::HandleInput(double dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Impulse(sf::Vector2f(dt * _speed, 0.f));
	}
}
PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size) : ActorPhysicsComponent(p, true, size)
{
	_size = Sv2_to_bv2(size, true);
	_maxVelocity = sf::Vector2f(200.f, 400.f);

	_speed = 30.f;

	_body->SetSleepingAllowed(false);
	_body->SetBullet(true); // Done for hi-res collision. Probably won't need it for the car
}

void PlayerPhysicsComponent::Update(double dt)
{
	HandleInput(dt);
	ActorPhysicsComponent::Update(dt);
}
