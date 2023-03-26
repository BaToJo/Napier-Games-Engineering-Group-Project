#include "Cmp_Player_Physics.h"
#include "..\lib_engine\System_Physics.h"
#include <SFML/Window/Keyboard.hpp>

using namespace Physics;

const std::vector<sf::Keyboard::Key> controls
{
	sf::Keyboard::W,
	sf::Keyboard::S,
	sf::Keyboard::A,
	sf::Keyboard::D
};
bool isPressed = false;

void PlayerPhysicsComponent::HandleDriving()
{
	float desiredSpeed = 0;

	if (sf::Keyboard::isKeyPressed(controls[0]))
	{
		desiredSpeed = _maxVelocity;
	}
	if (sf::Keyboard::isKeyPressed(controls[1]))
	{
		desiredSpeed = -_maxVelocity;
	}

	if (desiredSpeed == 0)
		return;
	// Get current speed in fwd dir
	b2Vec2 currentForwardNormal = _body->GetWorldVector(b2Vec2(0, 1));
	float currSpeed = b2Dot(getForwardVelocity(_body), currentForwardNormal);

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
	if (sf::Keyboard::isKeyPressed(controls[3]))
	{
		desiredTorque = -_maxTorque;
		isPressed = true;
	}
	if (sf::Keyboard::isKeyPressed(controls[2]))
	{
		desiredTorque = _maxTorque;
		isPressed = true;
	}
	

	_body->ApplyTorque(desiredTorque, true);
	
	for (int i = 0; i < _chain.size(); i++)
	{
		auto body = _chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		body->ApplyTorque(desiredTorque, true);
	}

}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain) : ActorPhysicsComponent(p, true, size), _wreckingBall(wreckingBall), _chain(chain)
{
	// Setting up Car's Body
	_size = Sv2_to_bv2(size, true);
	_body->SetSleepingAllowed(false);

	// Setting the first two dampings for the first two bodies.
	_body->SetAngularDamping(200.f); // Car's Angular Damping
	_body->SetLinearDamping(10.f);
	//chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(400.f); // First Chain's Angular Damping.

	
	// Defining the generic Joint Definition
	b2RevoluteJointDef chainRevoluteJointDef;

	//b2WeldJointDef weldJointDef;
	
	// How far apart are the chains?
	chainRevoluteJointDef.localAnchorA.Set(0, -0.35);
	chainRevoluteJointDef.localAnchorB.Set(0, 0.35);


	// They don't collide
	chainRevoluteJointDef.collideConnected = false;

	// Defining the first Joint that will anchor the chain to the car
	chainRevoluteJointDef.bodyA = _body;
	chainRevoluteJointDef.bodyB = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//weldJointDef.dampingRatio = 1;
	// Enabling the limit with no parameters will limit it to 0
	chainRevoluteJointDef.enableLimit = true;
	Physics::GetWorld()->CreateJoint(&chainRevoluteJointDef);

	for (int i = 1; i < 3; i++)
	{
		// Setting the up the chain bodies
		chainRevoluteJointDef.bodyA = chain[i-1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		chainRevoluteJointDef.bodyB = chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();

		// Setting the chain damping
		chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(10.f);
		//chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearDamping(5.f);

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
	ballJointDef.localAnchorB.Set(0, 0.85);

	// Setting the ball's damping
	wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(10.f);
	//wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearDamping(10.f);
	// Setting up the chain and the ball in the joint
	ballJointDef.bodyA = chain[chain.size() - 1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	ballJointDef.bodyB = wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// Limiting the ball so it won't spin around the anchor point
	ballJointDef.enableLimit = true;
	ballJointDef.collideConnected = false;
	// Storing the joint
	_ballJoint = static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&ballJointDef));

	for (auto& c : _chain)
	{
		auto body = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
		sumOfMasses += body->GetMass();
	}
}

void PlayerPhysicsComponent::Update(double dt)
{

	HandleDriving();
	HandleSteering();
	UpdateFriction();

	// This checks if we are skidding or not. If we stop skidding the chains will go back to rest
	if (getLateralVelocity(_body).Length() >= 1.2f)
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
	}
	for (int i = 0; i < _chain.size(); i++)
	{
		auto body = _chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();
		std::cout << "Chains lateral vel " << getLateralVelocity(body).Length() << std::endl;
		std::cout << "Chains fwd vel " << getForwardVelocity(body).Length() << std::endl;
		std::cout << "Chains angular vel " << body->GetAngularVelocity() << std::endl;
		//if (!sf::Keyboard::isKeyPressed(controls[3]) && !sf::Keyboard::isKeyPressed(controls[2]))
		//{
		//	body->SetLinearVelocity(_body->GetLinearVelocity());
		//	body->SetAngularVelocity(_body->GetAngularVelocity());
		//	
		//}

	}

	//if (!sf::Keyboard::isKeyPressed(controls[3]) && !sf::Keyboard::isKeyPressed(controls[2]))
	//{
	//	_wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetLinearVelocity(_body->GetLinearVelocity());
	//	_wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularVelocity(_body->GetAngularVelocity());
	//}

	//if(getLateralVelocity(_wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()) >)
	//if (isPressed && 
	//	(!sf::Keyboard::isKeyPressed(controls[3]) && !sf::Keyboard::isKeyPressed(controls[2])))
	//{
	//	for (auto& c : _chain)
	//	{
	//		auto body = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//		body->SetLinearVelocity(0.5f * body->GetLinearVelocity());
	//		body->SetAngularVelocity(0.5f * body->GetAngularVelocity());
	//	}

	//	isPressed = true;
	//}

	std::cout << "Body: Linear Velocity: " << _body->GetLinearVelocity().Length() << std::endl;
	std::cout << "Body: Lateral Velocity: " << getLateralVelocity(_body).Length() << std::endl;
	std::cout << "Body: Forward Velocity: " << getForwardVelocity(_body).Length() << std::endl;

	std::cout << "Body: Angular Velocity: " << _body->GetAngularVelocity() << std::endl;
	std::cout << "Body: Angular Impulse: " << _angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity() << std::endl;

	std::cout << std::endl;
	ActorPhysicsComponent::Update(dt);
}

b2Vec2 PlayerPhysicsComponent::getLateralVelocity(b2Body* body)
{
	b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 PlayerPhysicsComponent::getForwardVelocity(b2Body* body)
{
	b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;
}

void PlayerPhysicsComponent::UpdateFriction()
{
	// Skidding threshold
	float maxLateralImpulse = 0.04f;

	// Calculating the impulse to apply (this cancels out the lateral velocity)
	b2Vec2 impulseCar = _body->GetMass() * -getLateralVelocity(_body);

	// If the impulse we apply is greater than the threshold
	if (impulseCar.Length() > maxLateralImpulse)
	{
		// We modify the impulse of the car so it will start to skid
		impulseCar *= maxLateralImpulse / impulseCar.Length();
	}

	// Apply the linear impulse to the center of the car
	_body->ApplyLinearImpulse(impulseCar, _body->GetWorldCenter(), true);

	// Apply angular impulse damp
	_body->ApplyAngularImpulse(_angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity(), true);


	// Forward Linear Velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(_body);
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = _dragForceDamp * currentForwardSpeed;
	_body->ApplyForce(dragForceMagnitude * currentForwardNormal, _body->GetWorldCenter(), true);


	if (!sf::Keyboard::isKeyPressed(controls[3]) && !sf::Keyboard::isKeyPressed(controls[2]))
	{
		for (auto& c : _chain)
		{
			auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();

			b2Vec2 chainImpulse = chainBody->GetMass() * -getLateralVelocity(chainBody);
			chainBody->ApplyLinearImpulse(chainImpulse, chainBody->GetWorldCenter(), true);
			b2Vec2 currentChainFwd = getForwardVelocity(chainBody);

			float currentChainFwdSpd = currentChainFwd.Normalize();
			float dragForceMagnitudeChain = _dragForceDamp * currentChainFwdSpd;
			chainBody->ApplyForce(dragForceMagnitudeChain * currentChainFwd, chainBody->GetWorldCenter(), true);
		}

		auto ballBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
		

		b2Vec2 ballImpulse = ballBody->GetMass() * -getLateralVelocity(ballBody);
		ballBody->ApplyLinearImpulse(ballImpulse, ballBody->GetWorldCenter(), true);
		b2Vec2 currentChainFwd = getForwardVelocity(ballBody);

		float currentChainFwdSpd = currentChainFwd.Normalize();
		float dragForceMagnitudeChain = _dragForceDamp * currentChainFwdSpd;
		ballBody->ApplyForce(dragForceMagnitudeChain * currentChainFwd, ballBody->GetWorldCenter(), true);

	}





	
}
