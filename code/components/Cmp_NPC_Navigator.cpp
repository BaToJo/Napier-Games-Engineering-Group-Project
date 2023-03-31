#include "Cmp_NPC_Navigator.h"

#include "Cmp_Actor_Physics.h"
#include "..\lib_engine\System_Physics.h"


void NPCNavigatorComponent::Update(double dt)
{

	TurnVehicleToFacePoint(_current_destination);

	// If there is a vehicle ahead...
	if (Is_there_a_vehicle_ahead_of_me())
	{
		Halt();
	}
	else
	{
		Accelerate_to_Speed(30.0 /* This is just arbitrary, and should be replaced if different traffic speeds on different roads are implemented */);
	}


	/*if (!f->RayCast(&output, input))
		continue;
	if (output.fraction < closestFraction) {
		closestFraction = output.fraction;
		intersectionNormal = output.normal;
	}*/
}

void NPCNavigatorComponent::MoveTo(sf::Vector2f world_position)
{
	_current_destination = world_position;
}

void NPCNavigatorComponent::SetRaycastLength(double length)
{
	_raycast_length = length;
}

void NPCNavigatorComponent::Halt()
{
	// TODO: Make the vehicle decelerate to a halt. If it's stopped, remain stopped.
}

void NPCNavigatorComponent::Accelerate_to_Speed(float target_speed)
{
	// TODO: Accelerate the vehicle to a target speed.
}

void NPCNavigatorComponent::TurnVehicleToAngle(sf::Angle target_angle)
{
	// TODO: Turn the vehicle smoothly to face the target angle.
}

void NPCNavigatorComponent::TurnVehicleToFacePoint(sf::Vector2f target_point)
{
	// TODO: Turn the vehicle to smoothly track to point towards the target point.

	sf::Vector2f offset = target_point - _parent->getPosition();

	// If this is behaving strangely, check if it's interpreting degrees as radians, or vice versa.
	sf::Angle angle = sf::radians(offset.y / offset.x);

	TurnVehicleToAngle(angle);
}

bool NPCNavigatorComponent::Is_there_a_vehicle_ahead_of_me()
{
	bool at_least_one_vehicle_is_ahead_of_me = false;

	// Angular velocity of the vehicle in radians per second. 
	// float angular_velocity = vehicle->getComponents<ActorPhysicsComponent>()[0]->getBody()->GetAngularVelocity();

	// Originate the raycast p1 in the centre of the vehicle.
	_raycast_input.p1 = Physics::Sv2_to_bv2(_parent->getPosition());
	// Passing through the point p2
	_raycast_input.p2 = Physics::Sv2_to_bv2(sf::normalize(_current_destination - _parent->getPosition()));
	// How many times further than the distance from p1 to p2 should it check?
	_raycast_input.maxFraction = _raycast_length;

	// For each vehicle in the world...
	for (int i = 0; i < _all_vehicles->size(); i++)
	{
		// Cast a ray against it, and see if it hits. (I assume that Box2D checks if it is within the max distance and exits early if not, and doing such a check ourselves would duplicate work).

		if (_all_vehicles->at(i)->getComponents<ActorPhysicsComponent>()[0]->getBody()->GetFixtureList()[0].RayCast(
			&_raycast_output,
			_raycast_input,
			0 /* childIndex is only used in chain-type rigidbodies */))
		{
			at_least_one_vehicle_is_ahead_of_me = true;
		}
	}
	return at_least_one_vehicle_is_ahead_of_me;
}

NPCNavigatorComponent::NPCNavigatorComponent(Entity* p, std::shared_ptr<std::vector<std::shared_ptr<Entity>>> all_vehicles) : Component(p), _all_vehicles(all_vehicles) {}