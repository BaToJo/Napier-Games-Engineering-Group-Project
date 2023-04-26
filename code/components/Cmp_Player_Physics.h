#pragma once
#include "Cmp_Actor_Physics.h"

class PlayerPhysicsComponent : public ActorPhysicsComponent
{
protected:
	b2Vec2 _size;

	// Max velocity the car will go to
	float _maxVelocity = 200.f;

	// Maximum amount of force applied to the car
	float _maxForce = 100.f;

	// Forward velocity damping value
	float linearDamping = 0.5f;
	// Maximum amount of torque (how fast the car turns) applied to the car
	float _maxTorque = 5.f;

	// Damping constants for impulses and drag
	float _angularImpulseDamp = 0.1f;

	float _dragForceDamp = -2.f;

	// Skidding threshold
	float maxLateralImpulse = 0.1f;
	// Functions
	void HandleDriving();
	void HandleSteering();
	void UpdateFriction();

	// Getters
	b2Vec2 getLateralVelocity(b2Body* body);
	b2Vec2 getForwardVelocity(b2Body* body);

	std::shared_ptr<Entity> _wreckingBall;
	std::vector<std::shared_ptr<Entity>> _chain;

	std::vector<b2RevoluteJoint*> _chainJoints;
	b2RevoluteJoint* _ballJoint;

public:
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain);
	PlayerPhysicsComponent() = delete;

	void Update(double dt) override;
};