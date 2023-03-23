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

void PlayerPhysicsComponent::HandleDriving()
{
	float desiredSpeed = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		desiredSpeed = _maxVelocity;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		desiredSpeed = -_maxVelocity;
	}

	// Get current speed in fwd dir
	b2Vec2 currentForwardNormal = _body->GetWorldVector(b2Vec2(0, 1));
	float currSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

	float force = 0;
	if (desiredSpeed > currSpeed)
		force = _maxForce;
	else if (desiredSpeed < currSpeed)
		force = -_maxForce;
	else
		return;

	_body->ApplyForce(force * currentForwardNormal, _body->GetWorldCenter(), true);
}

void PlayerPhysicsComponent::HandleSteering()
{
	float desiredTorque = 0;
	//b2Vec2 desiredVel = b2Vec2(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		desiredTorque = -_maxTorque;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		desiredTorque = _maxTorque;

	}

	_body->ApplyTorque(desiredTorque, true);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain) : ActorPhysicsComponent(p, true, size), _wreckingBall(wreckingBall), _chain(chain)
{
	_size = Sv2_to_bv2(size, true);

	_body->SetSleepingAllowed(false);
	_body->SetBullet(true); // Done for hi-res collision. Probably won't need it for the car


	b2RevoluteJointDef rjd;
	rjd.localAnchorA.Set(0, -0.5);
	rjd.localAnchorB.Set(0, 0.5);

	rjd.bodyA = _body;
	rjd.bodyB = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	rjd.enableLimit = true;
	Physics::GetWorld()->CreateJoint(&rjd);

	for (int i = 1; i < 3; i++)
	{
		rjd.bodyA = chain[i-1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		rjd.bodyB = chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		rjd.enableLimit = false;
		rjd.upperAngle = (b2_pi / 180) * 90.f;
		rjd.lowerAngle = (b2_pi / 180) * -90.f;
		Physics::GetWorld()->CreateJoint(&rjd);
	}

	b2RevoluteJointDef ballJoint;
	ballJoint.localAnchorA.Set(0, 0);
	ballJoint.localAnchorB.Set(0, 1.4);

	ballJoint.bodyA = chain[chain.size() - 1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	ballJoint.bodyB = wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	Physics::GetWorld()->CreateJoint(&ballJoint);

}

void PlayerPhysicsComponent::Update(double dt)
{
	HandleDriving();
	HandleSteering();
	UpdateFriction();
	ActorPhysicsComponent::Update(dt);
}

b2Vec2 PlayerPhysicsComponent::getLateralVelocity()
{
	b2Vec2 currentRightNormal = _body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, _body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 PlayerPhysicsComponent::getForwardVelocity()
{
	b2Vec2 currentForwardNormal = _body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, _body->GetLinearVelocity()) * currentForwardNormal;
}

void PlayerPhysicsComponent::UpdateFriction()
{
	// Lateral Velocity
	float maxLateralImpulse = 0.04f;
	b2Vec2 impulse = _body->GetMass() * -getLateralVelocity();
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();
	
	_body->ApplyLinearImpulse(impulse, _body->GetWorldCenter(), true);

	// Angular Impulse
	_body->ApplyAngularImpulse(_angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity(), true);

	// Forward Linear Velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = _dragForceDamp * currentForwardSpeed;
	_body->ApplyForce(dragForceMagnitude * currentForwardNormal, _body->GetWorldCenter(), true);
}
