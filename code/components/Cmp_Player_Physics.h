#pragma once
#include "Cmp_Actor_Physics.h"

class PlayerPhysicsComponent : public ActorPhysicsComponent
{
protected:
	b2Vec2 _size;

	float _maxVelocity = 20.f;
	float _maxForce = 100.f;
	float _maxTorque = 15.f;

	float _angularImpulseDamp = 0.12f;
	float _dragForceDamp = -2.f;

	// Functions
	void HandleDriving();
	void HandleSteering();
	void UpdateFriction();

	// Getters
	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();

	std::shared_ptr<Entity> _wreckingBall;
	std::vector<std::shared_ptr<Entity>> _chain;

public:
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain);
	PlayerPhysicsComponent() = delete;

	void Update(double dt) override;
};