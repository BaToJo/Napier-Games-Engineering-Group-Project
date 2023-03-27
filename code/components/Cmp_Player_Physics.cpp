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

	_body->ApplyForce(0.5 * force * currentForwardNormal, _body->GetWorldCenter(), true);

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
	
	if (desiredTorque == 0)
		return;
	_body->ApplyTorque(desiredTorque, true);
	
	//for (int i = 0; i < _chain.size(); i++)
	//{
	//	auto body = _chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//	body->ApplyTorque(desiredTorque, true);
	//}

}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain) : ActorPhysicsComponent(p, true, size), _wreckingBall(wreckingBall), _chain(chain)
{
	// Setting up Car's Body
	_size = Sv2_to_bv2(size, true);
	_body->SetSleepingAllowed(false);

	////// Setting the first two dampings for the first two bodies.
	//_body->SetAngularDamping(50.f); // Car's Angular Damping
	//_body->SetLinearDamping(5.f); // Car's Linear Damping
	//// Define Rope Joint
	//b2RevoluteJointDef chainRevoluteJointDef;
	//chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(50.f);

	//chainRevoluteJointDef.collideConnected = true;
	//chainRevoluteJointDef.bodyA = _body;
	//chainRevoluteJointDef.bodyB = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();

	//chainRevoluteJointDef.localAnchorA.Set(0, -0.35);
	//chainRevoluteJointDef.localAnchorB.Set(0, 0.35);
	//chainRevoluteJointDef.enableLimit = true;

	//chainRevoluteJointDef.referenceAngle = 0;

	//_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainRevoluteJointDef)));
	////Physics::GetWorld()->CreateJoint(&frictionJointDef);
	//
	//for (int i = 1; i < 3; i++)
	//{
	//	chainRevoluteJointDef.bodyA = chain[i-1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//	chainRevoluteJointDef.bodyB = chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();

	//	chainRevoluteJointDef.localAnchorA.Set(0, -0.35);
	//	chainRevoluteJointDef.localAnchorB.Set(0, 0.35);
	//	chainRevoluteJointDef.enableLimit = true;

	//	chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(50.f);

	//	_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainRevoluteJointDef)));
	//}



	//b2RevoluteJointDef ballRevoluteJoint;

	//ballRevoluteJoint.bodyA = _chain[_chain.size() - 1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//ballRevoluteJoint.bodyB = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	//ballRevoluteJoint.localAnchorA.Set(0, 0);
	//ballRevoluteJoint.localAnchorB.Set(0, 0.85);
	//ballRevoluteJoint.enableLimit = true;

	//_ballJoint = static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&ballRevoluteJoint));















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

		chainRevoluteJointDef.localAnchorA.Set(0, -0.25);
		chainRevoluteJointDef.localAnchorB.Set(0, 0.25);

		// Setting the chain damping
		chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularDamping(200.f);
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
	};
}

void PlayerPhysicsComponent::Update(double dt)
{
	bool isAnyPressed = true;
	for (auto& c : controls)
	{
		if (sf::Keyboard::isKeyPressed(c))
		{
			isAnyPressed = true;
			break;
		}
		else
		{
			isAnyPressed = false;
		}

	}

	if (!isAnyPressed)
	{
		//std::cout << "No pressing" << std::endl;

		for (auto& c : _chain)
		{
			auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
			chainBody->SetAngularVelocity(0.f);
		}
		_body->SetAngularVelocity(0.f);
		_wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularVelocity(0.f);

	}

	if (!sf::Keyboard::isKeyPressed(controls[2]) && !sf::Keyboard::isKeyPressed(controls[3]))
	{
		for (auto& c : _chain)
		{
			auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
			chainBody->SetAngularVelocity(0.f);
			b2Vec2 impulseChain = chainBody->GetMass() * -getLateralVelocity(chainBody);
			chainBody->ApplyLinearImpulseToCenter(impulseChain, true);
		}

		auto wreckingBallBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
		b2Vec2 impulseBall = wreckingBallBody->GetMass() * -getLateralVelocity(wreckingBallBody);
		wreckingBallBody->ApplyLinearImpulseToCenter(impulseBall, true);
	}

	HandleDriving();
	HandleSteering();
	UpdateFriction();

	//for (auto& c : _chain)
	//{
	//	std::cout << "Chain angular " << c->getComponents<ActorPhysicsComponent>()[0]->getBody()->GetAngularVelocity() << std::endl;
	//	std::cout << "Chain lateral " << getLateralVelocity(c->getComponents<ActorPhysicsComponent>()[0]->getBody()).Length() << std::endl;
	//}




	//// This checks if we are skidding or not. If we stop skidding the chains will go back to rest
	//if (getLateralVelocity(_body).Length() >= .5f)
	//{
	//	for (auto& j : _chainJoints)
	//	{
	//		j->EnableLimit(false);
	//	}
	//}
	//else
	//{
	//	for (auto& j : _chainJoints)
	//	{
	//		j->EnableLimit(true);
	//	}
	//}

	//for (int i = 0; i < _chain.size(); i++)
	//{
	//	auto body = _chain[i]->getComponents<ActorPhysicsComponent>()[0]->getBody();

	//	std::cout << "Chain " << i << " lateral vel " << getLateralVelocity(body).Length() << std::endl;
	//	std::cout << "Chains " << i << " fwd vel " << getForwardVelocity(body).Length() << std::endl;
	//	std::cout << "Chains " << i << " angular vel " << body->GetAngularVelocity() << std::endl;

	//}

	//std::cout << "Body: Linear Velocity: " << _body->GetLinearVelocity().Length() << std::endl;
	//std::cout << "Body: Lateral Velocity: " << getLateralVelocity(_body).Length() << std::endl;
	//std::cout << "Body: Forward Velocity: " << getForwardVelocity(_body).Length() << std::endl;


	//std::cout << "Body: Angular Impulse: " << _angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity() << std::endl;

	//auto wreckingBallBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	//std::cout << "Wrecking Ball: Linear Velocity: " << wreckingBallBody->GetLinearVelocity().Length() << std::endl;
	//std::cout << "Wrecking Ball: Lateral Velocity: " << getLateralVelocity(wreckingBallBody).Length() << std::endl;
	//std::cout << "Wrecking Ball: Forward Velocity: " << getForwardVelocity(wreckingBallBody).Length() << std::endl;
	//std::cout << "Wrecking Ball: Angular Velocity: " << wreckingBallBody->GetAngularVelocity() << std::endl;
	//std::cout << std::endl;


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
	float maxLateralImpulse = 2.5f;

	// Calculating the impulse to apply (this cancels out the lateral velocity)
	b2Vec2 impulseCar = _body->GetMass() * -getLateralVelocity(_body);

	// If the impulse we apply is greater than the threshold
	if (impulseCar.Length() > maxLateralImpulse)
	{
		// We modify the impulse of the car so it will start to skid
		impulseCar *= maxLateralImpulse / impulseCar.Length();
	}

	// Apply the linear impulse to the center of the car
	_body->ApplyLinearImpulse(0.1 * impulseCar, _body->GetWorldCenter(), true);

	// Apply angular impulse damp
	_body->ApplyAngularImpulse(0.1 * _angularImpulseDamp * _body->GetInertia() * -_body->GetAngularVelocity(), true);


	// Forward Linear Velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(_body);
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = _dragForceDamp * currentForwardSpeed;
	_body->ApplyForce(0.1 * dragForceMagnitude * currentForwardNormal, _body->GetWorldCenter(), true);

	//auto wreckingBallBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//wreckingBallBody->ApplyAngularImpulse(0.1 * wreckingBallBody->GetInertia() * -wreckingBallBody->GetAngularVelocity(), true);

	//for (auto& c : _chain)
	//{
	//	auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//	chainBody->ApplyAngularImpulse(0.1 * chainBody->GetInertia() * -chainBody->GetAngularVelocity(), true);
	//}


	//if (wreckingBallBody->GetAngularVelocity() >= 10.f || wreckingBallBody->GetAngularVelocity() <= -10.f)
	//{
	//	for (auto& c : _chain)
	//	{
	//		auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//		chainBody->SetAngularDamping(20.f);
	//		chainBody->SetLinearDamping(4.f);
	//		/*b2Vec2 chainImpulse = chainBody->GetMass() * -getLateralVelocity(chainBody);
	//		chainBody->ApplyLinearImpulse(chainImpulse, chainBody->GetWorldCenter(), true);*/
	//	}

	//	wreckingBallBody->SetAngularDamping(40.f);
	//	wreckingBallBody->SetLinearDamping(4.f);
	//	/*b2Vec2 wreckingBallImpulse = wreckingBallBody->GetMass() * -getLateralVelocity(wreckingBallBody);
	//	wreckingBallBody->ApplyLinearImpulse(wreckingBallImpulse, wreckingBallBody->GetWorldCenter(), true);*/
	//	_body->SetAngularVelocity(0.f);
	//}
	//else 
	//{
	//	for (auto& c : _chain)
	//	{
	//		auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
	//		chainBody->SetAngularDamping(10.f);
	//		chainBody->SetLinearDamping(2.f);
	//	}
	//	wreckingBallBody->SetAngularDamping(20.f);
	//	wreckingBallBody->SetLinearDamping(4.f);
	//}




	//if (!sf::Keyboard::isKeyPressed(controls[3]) && !sf::Keyboard::isKeyPressed(controls[2]))
	//{
		//for (auto& c : _chain)
		//{
		//	auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();

		//	b2Vec2 chainImpulse = chainBody->GetMass() * -getLateralVelocity(chainBody);
		//	chainBody->ApplyLinearImpulse(chainImpulse, chainBody->GetWorldCenter(), true);
		//	b2Vec2 currentChainFwd = getForwardVelocity(chainBody);

		//	//float currentChainFwdSpd = currentChainFwd.Normalize();
		//	//float dragForceMagnitudeChain = _dragForceDamp * currentChainFwdSpd;
		//	//chainBody->ApplyForce(dragForceMagnitudeChain * currentChainFwd, chainBody->GetWorldCenter(), true);
		//}

		//auto ballBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
		//

		//b2Vec2 ballImpulse = ballBody->GetMass() * -getLateralVelocity(ballBody);
		//ballBody->ApplyLinearImpulse(ballImpulse, ballBody->GetWorldCenter(), true);
		//b2Vec2 currentChainFwd = getForwardVelocity(ballBody);

		//float currentChainFwdSpd = currentChainFwd.Normalize();
		//float dragForceMagnitudeChain = _dragForceDamp * currentChainFwdSpd;
		//ballBody->ApplyForce(dragForceMagnitudeChain * currentChainFwd, ballBody->GetWorldCenter(), true);

	//}





	
}
