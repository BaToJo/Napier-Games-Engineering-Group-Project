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
	//_body->SetBullet(true); // Done for hi-res collision. Probably won't need it for the car
	

	// Setting the first two dampings for the first two bodies.
	_body->SetAngularDamping(800.f);
	chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(400.f);
	chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearDamping(10.f);
	// Defining the generic Joint Definition
	b2RevoluteJointDef chainRevoluteJointDef;
	chainRevoluteJointDef.localAnchorA.Set(0, -0.40);
	chainRevoluteJointDef.localAnchorB.Set(0, 0.40);
	chainRevoluteJointDef.collideConnected = false;
	
	// Defining the first Joint that will anchor the chain to the car
	chainRevoluteJointDef.bodyA = _body;
	chainRevoluteJointDef.bodyB = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	chainRevoluteJointDef.enableLimit = true;
	Physics::GetWorld()->CreateJoint(&chainRevoluteJointDef);

	for (int i = 1; i < 3; i++)
	{
		// Setting the up the chain bodies
		chainRevoluteJointDef.bodyA = chain[i-1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		chainRevoluteJointDef.bodyB = chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();

		// Setting the chain damping
		chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(400.f - i * 100.f);
		chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearDamping(10.f - i * 2.f);

		// Limiting the angle - this makes the chain fixed
		chainRevoluteJointDef.enableLimit = true;
		chainRevoluteJointDef.upperAngle = (b2_pi / 180) * 0;
		chainRevoluteJointDef.lowerAngle = (b2_pi / 180) * 0;

		// Storing the joint
		_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainRevoluteJointDef)));
	}

	// Creating the ball joint
	b2RevoluteJointDef ballJointDef;
	ballJointDef.localAnchorA.Set(0, 0);
	ballJointDef.localAnchorB.Set(0, 0.75);

	// Setting the ball's damping
	wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(100.f);
	//wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearDamping(10.f);
	// Setting up the chain and the ball in the joint
	ballJointDef.bodyA = chain[chain.size() - 1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	ballJointDef.bodyB = wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// Limiting the ball so it won't spin around the anchor point
	ballJointDef.enableLimit = true;

	// Storing the joint
	_ballJoint = static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&ballJointDef));
}

void PlayerPhysicsComponent::Update(double dt)
{


	HandleDriving();
	HandleSteering();
	UpdateFriction();

	// This checks if we are drifting or not. If we stop drifting the chains will go back to rest

	if (getLateralVelocity().Length() >= 0.1f)
	{
		for (auto& j : _chainJoints)
		{
			j->EnableLimit(false);
		}

	}
	else
	{
		for (auto& j : _chainJoints)
		{
			j->EnableLimit(true);
		}

		//_body->SetAngularVelocity(0.f);
	}

	//std::cout << "Lateral Velocity: " << getLateralVelocity().Length() << std::endl;
	//std::cout << "Forward Velocity: " << getForwardVelocity().Length() << std::endl;
	//std::cout << "Linear Velocity: " << _body->GetLinearVelocity().Length() << std::endl;

	//std::cout << "Angular Velocity: " << _body->GetAngularVelocity() << std::endl;
	//std::cout << "Angular Impulse: " << _angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity() << std::endl;

	//std::cout << std::endl;
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

	if (getLateralVelocity().Length() >= 5.f)
	{
		_body->SetLinearDamping(1000.f);
	}
	else
	{
		_body->SetLinearDamping(5.f);
	}

	if (impulse.Length() > maxLateralImpulse)
	{
		impulse *= maxLateralImpulse / impulse.Length();
	}
	
	_body->ApplyLinearImpulse(impulse, _body->GetWorldCenter(), true);

	// Angular Impulse
	_body->ApplyAngularImpulse(_angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity(), true);


	// Forward Linear Velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = _dragForceDamp * currentForwardSpeed;
	_body->ApplyForce(dragForceMagnitude * currentForwardNormal, _body->GetWorldCenter(), true);
}
