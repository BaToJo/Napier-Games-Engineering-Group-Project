#pragma once
#include "Cmp_Actor_Physics.h"

class PlayerPhysicsComponent : public ActorPhysicsComponent
{
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	float _speed;

	void HandleInput(double dt);

public:
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);
	PlayerPhysicsComponent() = delete;

	void Update(double dt) override;
};