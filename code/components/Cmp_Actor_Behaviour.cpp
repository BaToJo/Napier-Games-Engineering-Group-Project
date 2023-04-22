#include "Cmp_Actor_Behaviour.h"

#include "Cmp_Waypoint.h"


//Entity* parent;
sf::Angle angle_to_waypoint_absolute;


AIBehaviourComponent::AIBehaviourComponent(Entity* p) : Component(p)
{
	//parent = _parent;
}

AIBehaviourComponent::~AIBehaviourComponent()
{
}

void AIBehaviourComponent::Update(double dt)
{
	float NPC_move_speed = 230;

	// The NPC attempts to drive directly towards its target waypoint.
	MoveToTargetOneTick(waypoint_destination->getPosition(), NPC_move_speed, dt);

	// When the NPC enters the trigger area for its target waypoint, it decides which waypoint from the list of destination waypoints to set as the next target waypoint.
	sf::Vector2f waypoint_offset = waypoint_destination->getPosition() - _parent->getPosition();
	float waypoint_trigger_radius = waypoint_destination->getComponents<AIWaypointComponent>()[0]->trigger_radius;
	float distance_to_waypoint = abs(waypoint_offset.length());
	if (distance_to_waypoint < waypoint_trigger_radius)
	{
		// Set the current waypoint to a new waypoint.
		std::shared_ptr<Entity> new_waypoint = waypoint_destination->getComponents<AIWaypointComponent>()[0]->GetRandomDestination();
		waypoint_most_recently_touched = waypoint_destination;
		waypoint_destination = new_waypoint;
	}

	// Any waypoint trigger that the NPC activates by being inside it, no matter whether it's the NPC's target or not, will become the new waypoint and the NPC will pick a legal destination.

	// The NPC does not register itself as inside the trigger of the most recent waypoint it's touched.

	// If the NPC's waypoint only allows one vehicle en-route to it and there's already a vehicle en-route, then halt and wait until that vehicle is no longer en-route to it.

}

sf::Angle AIBehaviourComponent::GetAngularOffset(sf::Angle azimuth)
{
	return sf::radians(azimuth.asRadians() - _parent->getRotation().asRadians());
}
sf::Angle AIBehaviourComponent::GetAngularOffset(sf::Vector2f target)
{
	sf::Vector2f offset = target - _parent->getPosition();
	if (offset.lengthSq() > 0)
	{
		return GetAngularOffset(offset.angle());
	}
	return sf::radians(0);
}

void AIBehaviourComponent::AimTowardsTargetOneTick(sf::Angle azimuth, double dt)
{
	// TODO: This is a placeholder that just sets the rotation directly with no physics. This should be updated with proper physics based turning.

	float turning_speed = 80.0f;

	float currentOrientation = _parent->getRotation().asDegrees();

	// Allow wrapping the angle around as sf::Angle doesn't allow negatives.
	float azimuth_degrees = azimuth.asDegrees();
	float wrapped_azimuth_negative = azimuth_degrees - 360;
	float wrapped_azimuth_positive = azimuth_degrees + 360;
	float nearest_azimuth = azimuth_degrees;
	if (abs(wrapped_azimuth_negative - currentOrientation) < abs(azimuth_degrees - currentOrientation))
	{
		nearest_azimuth = wrapped_azimuth_negative;
	}
	else if (abs(wrapped_azimuth_positive - currentOrientation) < abs(azimuth_degrees - currentOrientation))
	{
		nearest_azimuth = wrapped_azimuth_positive;
	}
	angle_to_waypoint_absolute = sf::degrees(abs(nearest_azimuth - currentOrientation));

	// This should turn this vehicle towards the specified azimuth.
	if (nearest_azimuth < currentOrientation)
	{
		sf::Angle newOrientation = sf::degrees(currentOrientation - (turning_speed * dt));
		_parent->setRotation(newOrientation);
	}
	else if (nearest_azimuth > currentOrientation)
	{
		sf::Angle newOrientation = sf::degrees(currentOrientation + (turning_speed * dt));
		_parent->setRotation(newOrientation);
	}
}
void AIBehaviourComponent::AimTowardsTargetOneTick(sf::Vector2f target, double dt)
{
	// This should turn this vehicle towards the target location.
	sf::Vector2f offset = target - _parent->getPosition();
	if (offset.lengthSq() > 0)
	{
		AimTowardsTargetOneTick(offset.angle(), dt);
	}
}

void AIBehaviourComponent::MoveToTargetOneTick(sf::Vector2f target, float move_speed, double dt)
{
	// TODO: This is a placeholder that just turns towards the target and moves forwards. This should be updated with proper physics based movement.

	AimTowardsTargetOneTick(target, dt);
	sf::Angle angularOffset = GetAngularOffset(target);
	float distance_to_move = (move_speed * dt) / (1 + angle_to_waypoint_absolute.asRadians());
	sf::Vector2f newPosition = sf::Vector2f(
		distance_to_move * cos(_parent->getRotation().asRadians()),
		distance_to_move * sin(_parent->getRotation().asRadians())
	);
	_parent->setPosition(_parent->getPosition() + newPosition);
}

