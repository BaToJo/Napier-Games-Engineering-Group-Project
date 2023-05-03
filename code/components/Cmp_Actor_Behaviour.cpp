#include "Cmp_Actor_Behaviour.h"

#include "Cmp_Actor_Physics.h"
#include "Cmp_Sprite.h"
#include "Cmp_Waypoint.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "../../lib_engine/System_Renderer.h"


//Entity* parent;
sf::Angle angle_to_waypoint_absolute;
sf::CircleShape debug_sensor;


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
		PhysicsTurnToAngle(sf::degrees(nearest_azimuth - currentOrientation), dt);
		//sf::Angle newOrientation = sf::degrees(currentOrientation - (turning_speed * dt));
		//_parent->setRotation(newOrientation);
	}
	else if (nearest_azimuth > currentOrientation)
	{
		PhysicsTurnToAngle(sf::degrees(nearest_azimuth - currentOrientation), dt);
		//sf::Angle newOrientation = sf::degrees(currentOrientation + (turning_speed * dt));
		//_parent->setRotation(newOrientation);
	}
}
float AIBehaviourComponent::HowCloseIsNearestCar()
{
	float vision_radius = 120.0f;


	float magnitude = vision_radius - 40.0f;
	sf::Angle angle = _parent->getRotation();
	sf::Vector2f offset = sf::Vector2f(magnitude * cos(angle.asRadians()), magnitude * sin(angle.asRadians()));
	sf::Vector2f sensor_position = _parent->getPosition() + offset;

	// TODO: Remove this debugging visualisation before release!
	//debug_sensor = sf::CircleShape(vision_radius);
	//debug_sensor.setFillColor(sf::Color::White);
	//debug_sensor.setOrigin(sf::Vector2f(vision_radius, vision_radius));
	//debug_sensor.setPosition(sensor_position);
	//Renderer::Queue(&debug_sensor);

	float shortest_distance = std::numeric_limits<float>::infinity();

	for (auto& vehicle : Engine::GetActiveScene()->ents.list)
	{
		// We don't want to sense ourself.
		if (vehicle.get() == _parent) continue;
		// We only want to sense entities with a physics component.
		if (vehicle->getComponents<ActorPhysicsComponent>().size() < 1) continue;
		// if (vehicle->getComponents<AIBehaviourComponent>().size() < 1) continue; // DEBUGGING TODO: Remove this.
		if (sf::length(vehicle->getPosition() - sensor_position) < vision_radius)
		{
			float distance = sf::length(vehicle->getPosition() - sensor_position);
			if (distance < shortest_distance)
			{
				shortest_distance = distance;
			}
		}
	}

	// std::cout << sf::length(vehicle->getPosition() - _parent->getPosition()) << std::endl;

	return shortest_distance;
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

	// if (HowCloseIsNearestCar() < 50) return;
	AimTowardsTargetOneTick(target, dt);
	// sf::Angle angularOffset = GetAngularOffset(target);
	float desired_speed = (move_speed * dt) / (1 + angle_to_waypoint_absolute.asRadians());
	float nearestCarDistance = HowCloseIsNearestCar();
	if (nearestCarDistance < 100)
	{
		desired_speed = desired_speed * ((nearestCarDistance - 30) / 70);
	}

	PhysicsDriveForwards(desired_speed, dt);

	//sf::Vector2f newPosition = sf::Vector2f(
	//	distance_to_move * cos(_parent->getRotation().asRadians()),
	//	distance_to_move * sin(_parent->getRotation().asRadians())
	//);
	//_parent->setPosition(_parent->getPosition() + newPosition);
}


void AIBehaviourComponent::PhysicsDriveForwards(float desired_velocity, double dt)
{
	float movement_power = 1.5f;

	desired_velocity = -desired_velocity * movement_power;

	if (_parent->getComponents<ActorPhysicsComponent>().size() < 1) return;
	b2Body* body = _parent->getComponents<ActorPhysicsComponent>()[0]->getBody();

	b2Vec2 current_forward_normal = body->GetWorldVector(b2Vec2(-1, 0));

	/*sf::Angle orientation = _parent->getRotation();
	b2Vec2 current_forward_normal = b2Vec2(cos(orientation.asRadians()), sin(orientation.asRadians()));*/

	b2Vec2 current_velocity = b2Dot(current_forward_normal, body->GetLinearVelocity()) * current_forward_normal;

	b2Vec2 desired_vector_velocity = desired_velocity * current_forward_normal;

	b2Vec2 velocityDifference = desired_vector_velocity - current_velocity;

	// F = ma
	// a = st

	b2Vec2 acceleration = b2Vec2(velocityDifference.x / dt, velocityDifference.y / dt);

	b2Vec2 force = body->GetMass() * acceleration;

	body->ApplyForceToCenter(force, true);
}

void AIBehaviourComponent::PhysicsTurnToAngle(sf::Angle delta_angle, double dt)
{
	if (_parent->getComponents<ActorPhysicsComponent>().size() < 1) return;
	b2Body* body = _parent->getComponents<ActorPhysicsComponent>()[0]->getBody();

	// torque = r * F * sin(theta)

	float radius = 10;
	float lever_force = 20;
	float torque = radius * lever_force * sin(delta_angle.asRadians());

	body->ApplyTorque(-torque, true);
}
