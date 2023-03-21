#pragma once
#include "Cmp_Actor_Physics.h"

class PlayerPhysicsComponent : public ActorPhysicsComponent
{
protected:
	b2Vec2 _size;

	float _maxVelocity = 20.f;
	float _maxForce = 50.f;
	float _maxTorque = 10.f;

	float _angularImpulseDamp = 0.12f;
	float dragForceDamp = -2.f;
	ActorPhysicsComponent* JointedBody;
	b2RevoluteJointDef rjd;

	// Functions
	void HandleDriving();
	void HandleSteering();
	void UpdateFriction();

	// Getters
	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();


public:
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, ActorPhysicsComponent* Ball);
	PlayerPhysicsComponent() = delete;

	void Update(double dt) override;
	//void AddJointedBody(ActorPhysicsComponent& physicsComp);
};