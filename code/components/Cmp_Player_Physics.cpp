#include "Cmp_Player_Physics.h"
#include "..\lib_engine\System_Physics.h"
#include "../lib_engine/Audio.h"
#include <SFML/Window/Keyboard.hpp>
#include "..\game.h"
using namespace Physics;

bool isPressed = false;

// The code implemented in this component has been partially taken from this source https://www.iforce2d.net/b2dtut/top-down-car
void PlayerPhysicsComponent::HandleDriving()
{
	float desiredSpeed = 0;

	// These multipliers are used to calculate how much a stick is being pressed. It defaults to 1 when keyboard is used
	float multiplierForward = InputManager::IsMovingForward();
	float multiplierBack = InputManager::IsMovingBack();

	// The multipliers are then used as a criteria for movement and also as a proper calculation that will influence the speed
	if (multiplierForward)
	{
		desiredSpeed = _maxVelocity * multiplierForward;
	}
	if (multiplierBack)
	{
		multiplierBack = multiplierBack == 1 ? -1 : multiplierBack;
		desiredSpeed = _maxVelocity * multiplierBack;
	}

	// Get current speed in fwd dir
	b2Vec2 currentForwardNormal = _body->GetWorldVector(b2Vec2(0, 1));
	float currSpeed = b2Dot(getForwardVelocity(_body), currentForwardNormal);

	// You should specify volume in the range 0 to 1, where 0 is silent and 1 is full volume.
	// You should specify pitch in the range 0 to 1, where 0.5 is half-pitch (lower/deeper) and 1.0 is normal.
	if (_settings[3].second)
	{
		float top_speed = 30;
		Audio::Sound_Set_Volume("engine_rev", abs(currSpeed / top_speed));
		Audio::Sound_Set_Volume("engine_idle", abs(1 - (currSpeed / top_speed)) * 0.2);
		Audio::Sound_Set_Pitch("engine_rev", abs(currSpeed / top_speed) / 2 + 0.5);
		Audio::Sound_Set_Pitch("engine_idle", abs(currSpeed / top_speed) / 2 + 0.5);
	}


	// If it's just 0, we return
	if (desiredSpeed == 0)
		return;


	// Set the force to 0, depending to the current speed of the car we set the force to the max force or to the negative of that

	float force = 0;
	if (desiredSpeed > currSpeed)
		force = _maxForce;
	else if (desiredSpeed < currSpeed)
		force = -_maxForce;
	else
		return;

	// Applying the force to the player
	_body->ApplyForce(linearDamping * force * currentForwardNormal, _body->GetWorldCenter(), true);

}

void PlayerPhysicsComponent::HandleSteering()
{
	float desiredTorque = 0;
	//b2Vec2 desiredVel = b2Vec2(0.f, 0.f);
	float multiplierRight = InputManager::IsMovingRight();
	float multiplierLeft = InputManager::IsMovingLeft();

	if (multiplierRight)
	{
		desiredTorque = _maxTorque * multiplierRight;
	}
	if (multiplierLeft)
	{
		multiplierLeft = multiplierLeft == 1 ? -1 : multiplierLeft;

		desiredTorque = _maxTorque * multiplierLeft;
	}


	_body->ApplyTorque(-desiredTorque, true);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, std::shared_ptr<Entity>& wreckingBall, std::vector<std::shared_ptr<Entity>>& chain) : ActorPhysicsComponent(p, true, size), _wreckingBall(wreckingBall), _chain(chain)
{
	// Setting up Car's Body
	_size = Sv2_to_bv2(size, true);
	_body->SetSleepingAllowed(false);
	_body->SetLinearDamping(5.f);

	// Defining a generic definition of a chain
	b2RevoluteJointDef chainJointDef;

	// FIRST JOINT - Joint between car and the first piece of the chain.
	chainJointDef.bodyA = _body;
	chainJointDef.bodyB = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// Setting up the anchor points
	chainJointDef.localAnchorA.Set(0, -0.4);
	chainJointDef.localAnchorB.Set(0, 0.4);
	// Setting up the limit to true so that the angle between the two bodies will stay 0
	chainJointDef.enableLimit = true;
	// Pushing back the created joint so that we can edit the joint properties later.
	_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainJointDef)));

	// SECOND JOINT - Joint between the first chain and the second chain
	chainJointDef.bodyA = chain[0]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	chainJointDef.bodyB = chain[1]->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// Here I don't reset anchorA and B and the enableLimit property because they are already being set from the previous declaration.
	_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainJointDef)));


	// THIRD JOINT - Joint between second chain and the wrecking ball
	chainJointDef.bodyA = chain[1]->getComponents<ActorPhysicsComponent>()[0]->getBody();
	chainJointDef.bodyB = wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// Resetting the anchor points to suit the wrecking ball best
	chainJointDef.localAnchorA.Set(0, -0.5);
	chainJointDef.localAnchorB.Set(0, 0.5);

	_chainJoints.push_back(static_cast<b2RevoluteJoint*>(Physics::GetWorld()->CreateJoint(&chainJointDef)));
}

void PlayerPhysicsComponent::Update(double dt)
{
	// Checking if any key is being pressed
	bool isAnyPressed = InputManager::IsMoving();

	// If no keys are being pressed
	if (!isAnyPressed)
	{
		// We get all chains in the list of chains and we set the angular velocity to 0
		for (auto& c : _chain)
		{
			auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
			chainBody->SetAngularVelocity(0.f);
		}
		// Same goes for body and wrecking ball
		_body->SetAngularVelocity(0.f);
		_wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->SetAngularVelocity(0.f);
	}

	// If we're not pressing A and D
	float movingLeft = InputManager::IsMovingLeft();
	float movingRight = InputManager::IsMovingRight();
	if (!movingLeft && !movingRight)
	{
		// Get every chain
		for (auto& c : _chain)
		{
			// Set angular velocity to 0
			auto chainBody = c->getComponents<ActorPhysicsComponent>()[0]->getBody();
			chainBody->SetAngularVelocity(0.f);
			// And generate an impulse that will cancel out the lateral velocity
			b2Vec2 impulseChain = chainBody->GetMass() * -getLateralVelocity(chainBody);
			chainBody->ApplyLinearImpulseToCenter(impulseChain, true);
		}

		// Same applies to the wrecking ball
		auto wreckingBallBody = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
		b2Vec2 impulseBall = wreckingBallBody->GetMass() * -getLateralVelocity(wreckingBallBody);
		wreckingBallBody->ApplyLinearImpulseToCenter(impulseBall, true);


		for (auto& j : _chainJoints)
			j->EnableLimit(true);

	}
	else
	{
		// If these buttons are pressed, then we release the joints so we can get the chain-like behaviour
		for (auto& j : _chainJoints)
			j->EnableLimit(false);
	}

	HandleDriving();
	HandleSteering();
	UpdateFriction();

	for (int i = 0; i < _chainJoints.size(); i++)
	{
		// Get the joint angle and convert it in degrees
		float angleDeg = _chainJoints[i]->GetJointAngle() * 180 / b2_pi;

		// If nothing is being pressed AND the angle is above the limits defined
		if (angleDeg > 25.f || angleDeg < -25.f && !isAnyPressed)
		{
			// Enable the motor and remove the limitation on the chain
			_chainJoints[i]->EnableMotor(true);
			_chainJoints[i]->EnableLimit(false);

			// Decide the direction and set the speed based on the given angle
			if (angleDeg > 0)
				_chainJoints[i]->SetMaxMotorTorque(100.f);
			_chainJoints[i]->SetMotorSpeed(50.f);
			if (angleDeg < 0)
				_chainJoints[i]->SetMaxMotorTorque(-100.f);
			_chainJoints[i]->SetMotorSpeed(-50.f);

		}
		else
		{
			// Disable the motor
			_chainJoints[i]->EnableMotor(false);
			_chainJoints[i]->EnableLimit(true);
			_chainJoints[i]->SetMaxMotorTorque(0.f);
			_chainJoints[i]->SetMotorSpeed(0.f);
		}
	}


	auto lateral_speed = _wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody()->GetLinearVelocity().Length();

	Audio::Sound_Set_Volume("sound_tire_screech_loop", lateral_speed * lateral_speed / 1800);
	Audio::Sound_Set_Pitch("sound_tire_screech_loop", lateral_speed * lateral_speed / 650);

	ActorPhysicsComponent::Update(dt);
}

b2Vec2 PlayerPhysicsComponent::getLateralVelocity(b2Body* body)
{
	// Get the lateral direction
	b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
	// Use the dotproduct to calculate the lateral velocity
	return b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 PlayerPhysicsComponent::getForwardVelocity(b2Body* body)
{
	// Get forward direction
	b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
	// Use the dotproduct to calculate the forward velocity
	return b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;
}

void PlayerPhysicsComponent::UpdateFriction()
{
	// Lateral Velocity
	float maxLateralImpulse = 2.5f;
	b2Vec2 impulse = _body->GetMass() * -getLateralVelocity(_body);
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();

	_body->ApplyLinearImpulse(impulse, _body->GetWorldCenter(), true);


	// Forward Linear Velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(_body);
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = _dragForceDamp * currentForwardSpeed;
	_body->ApplyForce(0.1 * dragForceMagnitude * currentForwardNormal, _body->GetWorldCenter(), true);

}
