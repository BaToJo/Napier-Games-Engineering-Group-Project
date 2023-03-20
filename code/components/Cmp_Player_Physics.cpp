#include "Cmp_Player_Physics.h"
#include "..\lib_engine\System_Physics.h"
#include <SFML/Window/Keyboard.hpp>

using namespace Physics;
const std::vector<sf::Keyboard::Key> controls
{
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Left,
	sf::Keyboard::Right
};
void PlayerPhysicsComponent::HandleInput(double dt)
{
	bool isAnyKeyPressed = false;
	for (int i = 0; i < controls.size(); i++)
	{
		if (sf::Keyboard::isKeyPressed(controls[i]))
		{
			isAnyKeyPressed = true;
		}
	}

	b2Vec2 vel = _body->GetLinearVelocity();
	b2Vec2 desiredVel = b2Vec2(0.f, 0.f);
	if (isAnyKeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			desiredVel.x = b2Min(vel.x + 2.f, 5.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			desiredVel.x = b2Max(vel.x - 2.f, -5.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			desiredVel.y = b2Max(vel.y + 2.f, 5.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			desiredVel.y = b2Min(vel.y - 2.f, -5.f);
		}
	}
	else
	{
		desiredVel = 0.1f * vel;
	}

	b2Vec2 velChange = desiredVel - vel;
	b2Vec2 impulse = dt * _body->GetMass() * velChange;
	_body->ApplyLinearImpulseToCenter(impulse, true);

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
	//UpdateFriction();
	ActorPhysicsComponent::Update(dt);
}

b2Vec2 PlayerPhysicsComponent::getLateralVelocity()
{
	b2Vec2 currentRightNormal = _body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, _body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 PlayerPhysicsComponent::getHorizontalVelocity()
{
	b2Vec2 currentForwardNormal = _body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, _body->GetLinearVelocity()) * currentForwardNormal;
}

void PlayerPhysicsComponent::UpdateFriction()
{
	b2Vec2 impulse = _body->GetMass() * -getLateralVelocity();
	
	_body->ApplyLinearImpulse(impulse, _body->GetWorldCenter(), true);
}
