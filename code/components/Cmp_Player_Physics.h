#pragma once
#include "Cmp_Actor_Physics.h"

class PlayerPhysicsComponent : public ActorPhysicsComponent
{
protected:
	b2Vec2 _size;
	sf::Vector2f _maxVelocity;
	float _speed;
	ActorPhysicsComponent* JointedBody;
	b2RevoluteJointDef rjd;

	void HandleDriving();
	void HandleSteering();
	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();
	void UpdateFriction();
public:
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, ActorPhysicsComponent* Ball);
	PlayerPhysicsComponent() = delete;

	void Update(double dt) override;
	//void AddJointedBody(ActorPhysicsComponent& physicsComp);
};